#include <stdlib.h>
#include <string.h>
#include "scope.h"
#include <stdio.h>

/* args helpers */
arg_type_node_t *args_append(arg_type_node_t *head, data_type_t type, const char *name) {
  arg_type_node_t *node = (arg_type_node_t*)calloc(1, sizeof(arg_type_node_t));
  if (!node) return head;
  node->type = type;

  // Stores a copy of the name
  if (name) {
    node->name = strdup(name);
  } else {
    node->name = NULL;
  }

  if (!head) return node;
  arg_type_node_t *cur = head;
  while (cur->next) cur = cur->next;
  cur->next = node;
  return head;
}

void args_free(arg_type_node_t *head) {
  while (head) {
    arg_type_node_t *nxt = head->next;
    // Frees name copy created in args_append with strdup
    if (head->name) free(head->name);
    free(head);
    head = nxt;
  }
}

/* symbol table */
symbol_table_t *symtab_create(void) {
  symbol_table_t *t = (symbol_table_t*)calloc(1, sizeof(symbol_table_t));
  return t;
}

void symtab_destroy(symbol_table_t *table) {
  if (!table) return;
  symbol_entry_t *cur = table->head;
  while (cur) {
    symbol_entry_t *nxt = cur->next;
    free(cur->key);
    if (cur->lexical.value) free(cur->lexical.value);
    args_free(cur->args);
    free(cur);
    cur = nxt;
  }
  free(table);
}

symbol_entry_t *symtab_lookup_local(symbol_table_t *table, const char *key) {
  if (!table || !key) return NULL;
  for (symbol_entry_t *e = table->head; e; e = e->next) {
    if (strcmp(e->key, key) == 0) return e;
  }
  return NULL;
}

symbol_entry_t *symtab_insert(symbol_table_t *table,
                              const char *key,
                              symbol_nature_t nature,
                              data_type_t data_type,
                              const lexical_value_t *lexical_opt) {
  if (!table || !key) return NULL;
  if (symtab_lookup_local(table, key)) return NULL; /* duplicate in same scope */
  symbol_entry_t *e = (symbol_entry_t*)calloc(1, sizeof(symbol_entry_t));
  if (!e) return NULL;
  e->key = strdup(key);
  e->nature = nature;
  e->data_type = data_type;
  e->args = NULL;
  e->lexical.line_number = lexical_opt ? lexical_opt->line_number : 0;
  e->lexical.token_type = lexical_opt ? lexical_opt->token_type : NULL;
  e->lexical.value = lexical_opt && lexical_opt->value ? strdup(lexical_opt->value) : NULL;
  e->next = table->head;
  table->head = e;
  return e;
}

void symbol_entry_add_arg(symbol_entry_t *entry, data_type_t type) {
  if (!entry) return;
  entry->args = args_append(entry->args, type, entry->key);
}

/* scope stack */
scope_stack_t *scope_stack_create(void) {
  scope_stack_t *s = (scope_stack_t*)calloc(1, sizeof(scope_stack_t));
  return s;
}

void scope_stack_destroy(scope_stack_t *stack) {
  if (!stack) return;
  while (stack->top) {
    scope_pop(stack);
  }
  free(stack);
}

void scope_push(scope_stack_t *stack, scope_kind_t kind) {
  if (!stack) return;
  scope_t *sc = (scope_t*)calloc(1, sizeof(scope_t));
  if (!sc) return;
  sc->kind = kind;
  sc->table = symtab_create();
  sc->next = stack->top;
  stack->top = sc;
}

void scope_pop(scope_stack_t *stack) {
  if (!stack || !stack->top) return;
  scope_t *top = stack->top;
  stack->top = top->next;
  symtab_destroy(top->table);
  free(top);
}

scope_t *scope_current(scope_stack_t *stack) {
  return stack ? stack->top : NULL;
}

symbol_entry_t *scope_lookup(scope_stack_t *stack, const char *key) {
  if (!stack || !key) return NULL;
  for (scope_t *sc = stack->top; sc; sc = sc->next) {
    symbol_entry_t *e = symtab_lookup_local(sc->table, key);
    if (e) return e;
  }
  return NULL;
}

symbol_entry_t *scope_insert_current(scope_stack_t *stack,
                                     const char *key,
                                     symbol_nature_t nature,
                                     data_type_t data_type,
                                     const lexical_value_t *lexical_opt) {
  scope_t *cur = scope_current(stack);
  if (!cur) return NULL;
  return symtab_insert(cur->table, key, nature, data_type, lexical_opt);
}

/* ----------------- logging ----------------- */
static const char *scope_kind_str(scope_kind_t k) {
  switch (k) {
    case SCOPE_GLOBAL: return "GLOBAL";
    case SCOPE_FUNCTION: return "FUNCTION";
    case SCOPE_BLOCK: return "BLOCK";
    default: return "UNKNOWN";
  }
}

static const char *nature_str(symbol_nature_t n) {
  switch (n) {
    case SYMBOL_LITERAL: return "literal";
    case SYMBOL_VARIABLE: return "variable";
    case SYMBOL_FUNCTION: return "function";
    default: return "?";
  }
}

static const char *dtype_str(data_type_t t) {
  switch (t) {
    case TYPE_INT: return "int";
    case TYPE_FLOAT: return "float";
    case TYPE_UNDEFINED: default: return "undef";
  }
}

static void print_args_list(FILE *out, const arg_type_node_t *args) {
  fprintf(out, "(");
  const arg_type_node_t *cur = args;
  int first = 1;
  while (cur) {
    if (!first) fprintf(out, ", ");
    const char *name = cur->name ? cur->name : "_";
    fprintf(out, "%s %s", dtype_str(cur->type), name);
    first = 0;
    cur = cur->next;
  }
  fprintf(out, ")");
}

static void symtab_print_with_header(FILE *out, scope_t *sc, const char *header) {
  if (!sc || !sc->table) return;
  fprintf(out, "===== %s SCOPE [%s] =====\n", header, scope_kind_str(sc->kind));
  for (symbol_entry_t *e = sc->table->head; e; e = e->next) {
    fprintf(out, "- %s: key='%s' type=%s", nature_str(e->nature), e->key ? e->key : "",
            dtype_str(e->data_type));
    if (e->nature == SYMBOL_FUNCTION) {
      fprintf(out, " args=");
      print_args_list(out, e->args);
    }
    if (e->lexical.value) {
      fprintf(out, " lexval='%s'", e->lexical.value);
    }
    if (e->lexical.token_type) {
      fprintf(out, " token=%s", e->lexical.token_type);
    }
    if (e->lexical.line_number) {
      fprintf(out, " line=%d", e->lexical.line_number);
    }
    fprintf(out, "\n");
  }
  fprintf(out, "==============================\n");
}

static void scope_log_top(scope_stack_t *stack, const char *header) {
  if (!stack || !stack->top) return;
  symtab_print_with_header(stderr, stack->top, header);
}

void scope_log_global_end(scope_stack_t *stack) {
  scope_log_top(stack, "END OF");
}

void scope_log_function_end(scope_stack_t *stack) {
  scope_log_top(stack, "END OF");
}

void scope_log_block_end(scope_stack_t *stack) {
  scope_log_top(stack, "END OF");
}

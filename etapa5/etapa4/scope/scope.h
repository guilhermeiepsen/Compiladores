#ifndef SCOPE_H
#define SCOPE_H

#include <stddef.h>
#include "lexical_value.h"

/* We begin the type definition from leaf to root... */

// Basic definition of symbol nature
typedef enum {
  SYMBOL_LITERAL = 1,
  SYMBOL_VARIABLE = 2,
  SYMBOL_FUNCTION = 3
} symbol_nature_t;

// Basic definition of symbol type
typedef enum {
  TYPE_UNDEFINED = 0,
  TYPE_INT = 1,
  TYPE_FLOAT = 2
} data_type_t;

typedef struct arg_type_node {
  data_type_t type;
  char *name;
  struct arg_type_node *next;
} arg_type_node_t;

/*
 * Entradas da tabela de símbolos
 */
typedef struct symbol_entry {
  char *key;                       /* unique key for the symbol */
  symbol_nature_t nature;          /* literal, variable, function */
  data_type_t data_type;           /* int or float */
  arg_type_node_t *args;           /* only for functions, NULL otherwise */
  lexical_value_t lexical;         /* token data (line, token_type, value copy) */

  // E5: Offset de memória para variáveis
  // Se for global: offset relativo a rbss
  // Se for local: offset relativo a rfp
  int offset; 

  struct symbol_entry *next;       /* next entry in this scope's list */
} symbol_entry_t;

/*
 * Tabela de Símbolos
 */
typedef struct symbol_table {
  symbol_entry_t *head;            /* singly linked list of entries */
  
  // E5: Contador de offset local para este escopo específico
  // Útil para saber qual o próximo endereço disponível neste escopo
  int current_offset; 
} symbol_table_t;

typedef enum {
  SCOPE_GLOBAL = 1,
  SCOPE_FUNCTION = 2,
  SCOPE_BLOCK = 3
} scope_kind_t;

typedef struct scope {
  scope_kind_t kind;
  symbol_table_t *table;           /* table for this scope */
  struct scope *next;              /* next scope in stack (below current) */
} scope_t;

typedef struct scope_stack {
  scope_t *top;                    /* top of the stack */
} scope_stack_t;

/* symbol table api */
symbol_table_t *symtab_create(void);
void symtab_destroy(symbol_table_t *table);
symbol_entry_t *symtab_lookup_local(symbol_table_t *table, const char *key);
symbol_entry_t *symtab_insert(symbol_table_t *table,
                              const char *key,
                              symbol_nature_t nature,
                              data_type_t data_type,
                              const lexical_value_t *lexical_opt);
void symbol_entry_add_arg(symbol_entry_t *entry, data_type_t type, const char *name);

/* scope stack api */
scope_stack_t *scope_stack_create(void);
void scope_stack_destroy(scope_stack_t *stack);
void scope_push(scope_stack_t *stack, scope_kind_t kind);
void scope_pop(scope_stack_t *stack);
scope_t *scope_current(scope_stack_t *stack);
symbol_entry_t *scope_lookup(scope_stack_t *stack, const char *key);
symbol_entry_t *scope_insert_current(scope_stack_t *stack,
                                     const char *key,
                                     symbol_nature_t nature,
                                     data_type_t data_type,
                                     const lexical_value_t *lexical_opt);

/* helpers for args list */
arg_type_node_t *args_append(arg_type_node_t *head, data_type_t type, const char *name);
void args_free(arg_type_node_t *head);

/* helper para AST */
static inline symbol_entry_t *sym_select_head_and_attach_tail(symbol_entry_t *head, symbol_entry_t *tail) {
  if (head != NULL) return head; else return tail;
}

/* logging helpers */
void scope_log_global_end(scope_stack_t *stack);
void scope_log_function_end(scope_stack_t *stack);
void scope_log_block_end(scope_stack_t *stack);

#endif /* SCOPE_H */
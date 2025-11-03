#ifndef SCOPE_H
#define SCOPE_H

#include <stddef.h>
#include "lexical_value.h"

/*
USAGE EXEMPLE::
#include "scope/scope.h"

scope_stack_t *stack = scope_stack_create();
scope_push(stack, SCOPE_GLOBAL);
scope_insert_current(stack, "x", SYMBOL_IDENTIFIER, TYPE_INT, NULL);
symbol_entry_t *e = scope_lookup(stack, "x"); // encontra "x"
scope_pop(stack);
scope_stack_destroy(stack);
*/

/* We begin the type definition from leaf to root... */

// Basic definition of symbol nature
typedef enum {
  SYMBOL_LITERAL = 1,
  SYMBOL_VARIABLE = 2,
  SYMBOL_FUNCTION = 3
} symbol_nature_t;

// Basic definition of symbol type (don't know if UNDEFINED should be here...)
typedef enum {
  TYPE_UNDEFINED = 0,
  TYPE_INT = 1,
  TYPE_FLOAT = 2
} data_type_t;

/*
By the definition: "argumentos e seus tipos (no caso de funções)"
it seems that we need to store:
 the arg_name (witch should be in the function scope too)
 the type (important for typechecking later...)
*/
typedef struct arg_type_node {
  data_type_t type;
  char *name;
  struct arg_type_node *next;
} arg_type_node_t;

/*
A symbol in the symbol table has,according to the definition::
• natureza (literal, variable ou função) - symbol_nature_t nature
• tipo do dado do símbolo (int ou float) - data_type_t data_type
• argumentos e seus tipos (no caso de funções) - arg_type_node_t *args - pointer for the args list
• dados do valor do token pelo yylval (veja E3) - lexical_value_t lexical

** Obviously a pointer to the next node on table
*/
typedef struct symbol_entry {
  char *key; //AKA name            /* unique key for the symbol */
  symbol_nature_t nature;          /* literal, variable, function */
  data_type_t data_type;           /* int or float */
  arg_type_node_t *args;           /* only for functions, NULL otherwise */
  lexical_value_t lexical;         /* token data (line, token_type, value copy) */

  struct symbol_entry *next;       /* next entry in this scope's list */
} symbol_entry_t;

/*
This symbol_table_t is what we store in the scope stack, a pointer to each scope table of symbols
*/
typedef struct symbol_table {
  symbol_entry_t *head;            /* singly linked list of entries */
} symbol_table_t;

/*
Definition of scope type (don't know if strictly necessary but its good for aut documenting the scopes)
*/
typedef enum {
  SCOPE_GLOBAL = 1,
  SCOPE_FUNCTION = 2,
  SCOPE_BLOCK = 3
} scope_kind_t;

/*
Scope node having a symple scope type, a pointer to head of symbol table and a pointer to BELOW SCOPE
THERE IS NO NEED FOR US TO HAVE A POINTER TO A HIGHER SCOPE SINCE WE ONLY WILL SEARCH FOR VARIABLES DEFINED
IN OUTER SCOPES

OUTER -> MORE BROAD -> BELOW IN THE TABLE -> RECEIVING END OF POINTERS IN STACK
INNER -> MORE SPECIFIC -> ABOVE IN THE TABLE -> EMITTING END OF POINTER
*/
typedef struct scope {
  scope_kind_t kind;
  symbol_table_t *table;           /* table for this scope */
  struct scope *next;              /* next scope in stack (below current) */
} scope_t;

/* The scope base, simple way to see if the stack is empty (SHOULD NEVER BE, ALWAYS HAVE THE GLOBAL SCOPE) */
typedef struct scope_stack {
  scope_t *top;                    /* top of the stack */
} scope_stack_t;

/* symbol table api */
symbol_table_t *symtab_create(void);
void symtab_destroy(symbol_table_t *table);
// Permits searching inside scope for a variable definition
symbol_entry_t *symtab_lookup_local(symbol_table_t *table, const char *key);
symbol_entry_t *symtab_insert(symbol_table_t *table,
                              const char *key,
                              symbol_nature_t nature,
                              data_type_t data_type,
                              const lexical_value_t *lexical_opt);
void symbol_entry_add_arg(symbol_entry_t *entry, data_type_t type, const char *name); //modificado para adicionar nome do argumento

/* scope stack api */
scope_stack_t *scope_stack_create(void);
void scope_stack_destroy(scope_stack_t *stack);
void scope_push(scope_stack_t *stack, scope_kind_t kind);
void scope_pop(scope_stack_t *stack);
scope_t *scope_current(scope_stack_t *stack);
// Permits searching for a variable in all the BROADER SCOPES, searches recursively(not quite because calls lookup_local) in scopes
symbol_entry_t *scope_lookup(scope_stack_t *stack, const char *key);
symbol_entry_t *scope_insert_current(scope_stack_t *stack,
                                     const char *key,
                                     symbol_nature_t nature,
                                     data_type_t data_type,
                                     const lexical_value_t *lexical_opt);

/* helpers for args list */
arg_type_node_t *args_append(arg_type_node_t *head, data_type_t type, const char *name); //modificado para adicionar nome do argumento
void args_free(arg_type_node_t *head);

/* selection helper analogous to parser rules: choose head, attach tail, else return tail */
/* mirrors asd_select_head_and_attach_tail but kept here for semantic structures if needed */
static inline symbol_entry_t *sym_select_head_and_attach_tail(symbol_entry_t *head, symbol_entry_t *tail) {
  if (head != NULL) return head; else return tail;
}

/* logging helpers: print the top scope's table before popping */
void scope_log_global_end(scope_stack_t *stack);
void scope_log_function_end(scope_stack_t *stack);
void scope_log_block_end(scope_stack_t *stack);

#endif /* SCOPE_H */


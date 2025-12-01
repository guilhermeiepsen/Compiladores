%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asd.h"
#include "scope.h"
#include "iloc.h"

extern int yylineno;
int yylex(void);
void yyerror (char const *mensagem);
extern char *yytext;
extern asd_tree_t *arvore;
scope_stack_t *scope_stack_pointer = NULL;

/* Função de erro semântico centralizada */
void semantic_error(int error_code, int line, const char *text) {
    printf("Semantic ERROR at line %d: %s (Code: %d)\n", line, text ? text : "(null)", error_code);
    exit(error_code);
}

/* * HELPER DE DEBUG (E5) */
static int get_reg_id(const char *temp_str) {
    if (!temp_str) {
        fprintf(stderr, "FATAL ERROR: get_reg_id called with NULL pointer. Logic error in AST code gen.\n");
        exit(1);
    }
    if (temp_str[0] != 'r') {
        fprintf(stderr, "FATAL ERROR: Invalid register format: '%s'\n", temp_str);
        exit(1);
    }
    return atoi(temp_str + 1);
}
%}

%code requires {
 #include "asd.h"
 #include "lexical_value.h"
 #include "scope.h"
 #include "iloc.h"
}
%define parse.error verbose

%union {
 asd_tree_t *node;
 lexical_value_t lexical_value;
 arg_type_node_t *arg_list;
 data_type_t data_type;
 symbol_entry_t *symbol;
}

/* DESTRUTOR REMOVIDO PARA EVITAR CORRUPÇÃO DE TOKENS EM ALGUMAS VERSÕES */

/* Tipos das regras */
%type <node> program list element function_definition body command_block command_sequence simple_command
%type <node> variable_declaration variable_declaration_with_instantiation optional_instantiation literal
%type <node> attribution_command function_call args return_command flow_control_command
%type <node> conditional_struct else_block iterative_struct
%type <node> expression logical_or_expression logical_and_expression equality_expression
%type <node> relational_expression additive_expression multiplicative_expression unary_expression primary_expression
%type <node> global_escope_init global_escope_end block_scope_init

%type <data_type> var_type
%type <arg_list> optional_parameter_list parameter_list parameter
%type <symbol> header

/* Tokens */
%token TK_TIPO TK_VAR TK_SENAO TK_DECIMAL TK_SE TK_INTEIRO TK_ATRIB TK_RETORNA TK_SETA TK_ENQUANTO TK_COM
%token TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%token <lexical_value>TK_ID TK_LI_INTEIRO TK_LI_DECIMAL TK_ER

%%

/* ========================================================================== */
/* PROGRAMA E ESCOPO GLOBAL                       */
/* ========================================================================== */

program: global_escope_init list global_escope_end ';' {
  arvore = $2;
  // E5: Imprimir o código gerado na saída padrão
  if (arvore && arvore->code) {
      iloc_print_code(arvore->code);
  }
};

global_escope_init: %empty { 
  $$ = NULL;
  if (!scope_stack_pointer) scope_stack_pointer = scope_stack_create();
  scope_push(scope_stack_pointer, SCOPE_GLOBAL);
 };

global_escope_end: %empty {
  $$ = NULL;
  scope_log_global_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
  scope_stack_destroy(scope_stack_pointer);
  scope_stack_pointer = NULL;
 };

list: element {
  $$ = $1;
}
| element ',' list {
  $$ = asd_select_head_and_attach_tail($1, $3);
  if ($1 && $3) {
      $1->code = iloc_append($1->code, $3->code);
  }
};

element: function_definition { $$ = $1; }
| variable_declaration { $$ = $1; };

/* ========================================================================== */
/* DEFINIÇÃO DE FUNÇÕES E PARÂMETROS                   */
/* ========================================================================== */

function_definition: header {
  scope_push(scope_stack_pointer, SCOPE_FUNCTION);
  for(arg_type_node_t *arg = $1->args; arg; arg = arg->next) {
    lexical_value_t param_lex;
    param_lex.line_number = $1->lexical.line_number;
    param_lex.token_type = "param";
    param_lex.value = arg->name; 

    symbol_entry_t *param_entry = scope_insert_current(scope_stack_pointer, arg->name, SYMBOL_VARIABLE, arg->type, &param_lex);

    if(param_entry == NULL) {
      semantic_error(11, param_lex.line_number, arg->name); // ERR_DECLARED
    }
  }
} body {
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "function %s", $1->key);
  $$ = asd_new(buffer);
  $$->type = $1->data_type;

  asd_add_child($$, $3);

  if ($3) $$->code = $3->code;

  scope_log_function_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
};

header: TK_ID TK_SETA var_type optional_parameter_list TK_ATRIB {
  symbol_entry_t *entry = scope_insert_current(scope_stack_pointer, $1.value, SYMBOL_FUNCTION, $3, &$1);
  if(entry == NULL) {
    semantic_error(11, $1.line_number, $1.value); // ERR_DECLARED
  }
  entry->args = $4;
  $$ = entry;
  free($1.value);
};

var_type: TK_DECIMAL { $$ = TYPE_FLOAT; }
| TK_INTEIRO { $$ = TYPE_INT; };

optional_parameter_list: %empty { $$ = NULL; }
| TK_COM parameter_list { $$ = $2; }
| parameter_list { $$ = $1; };

parameter_list: parameter {
  $$ = $1; 
}
| parameter_list ',' parameter {
  arg_type_node_t *head = $1;
  arg_type_node_t *cur = head;
  while (cur && cur->next) cur = cur->next;
  if (cur) cur->next = $3;
  $$ = head;
};

parameter: TK_ID TK_ATRIB var_type {
  $$ = NULL;
  $$ = args_append($$, $3, $1.value);
  free($1.value);
};

/* ========================================================================== */
/* BLOCO DE COMANDOS E ESCOPO LOCAL                    */
/* ========================================================================== */

body: command_block { $$ = $1; };

command_block: block_scope_init command_sequence ']' {
  $$ = $2;
  scope_log_block_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
}
| block_scope_init ']' {
  $$ = NULL;
  scope_log_block_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
};

block_scope_init: '[' {
  $$ = NULL;
  scope_push(scope_stack_pointer, SCOPE_BLOCK);
};

command_sequence: simple_command {
  $$ = $1;
}
| simple_command command_sequence {
  $$ = asd_select_head_and_attach_tail($1, $2);
  if ($1 && $2) {
      $1->code = iloc_append($1->code, $2->code);
  }
};

simple_command: variable_declaration_with_instantiation { $$ = $1; }
| function_call { $$ = $1; }
| attribution_command { $$ = $1; }
| return_command { $$ = $1; }
| flow_control_command { $$ = $1; }
| command_block { $$ = $1; };

/* ========================================================================== */
/* DECLARAÇÃO DE VARIÁVEIS                        */
/* ========================================================================== */

variable_declaration: TK_VAR TK_ID TK_ATRIB var_type {
  const data_type_t data_type = $4;
  if(scope_insert_current(scope_stack_pointer, $2.value, SYMBOL_VARIABLE, data_type, &$2) == NULL) {
    semantic_error(11, $2.line_number, $2.value); // ERR_DECLARED
  }
  $$ = NULL;  
  free($2.value);
};

variable_declaration_with_instantiation: TK_VAR TK_ID TK_ATRIB var_type optional_instantiation {
  const data_type_t var_type = $4;
  symbol_entry_t *entry = scope_insert_current(scope_stack_pointer, $2.value, SYMBOL_VARIABLE, var_type, &$2);
  
  if (entry == NULL) {
    semantic_error(11, $2.line_number, $2.value); // ERR_DECLARED
  }

  asd_tree_t *inst_node = $5;
  if (inst_node != NULL) {
    if (inst_node->type != var_type) {
      semantic_error(30, $2.line_number, "Instantiation type mismatch"); // ERR_WRONG_TYPE
    }
    
    // E5: Geração de código para inicialização (storeAI)
    // Se for global -> rbss (-2), se local -> rfp (-1)
    int base_reg = entry->is_global ? -2 : -1;
    
    iloc_code_t *store = iloc_create_inst(ILOC_STOREAI,
                                          ILOC_OP_REG, get_reg_id(inst_node->temp),
                                          ILOC_OP_REG, base_reg,
                                          ILOC_OP_INT, entry->offset);
    inst_node->code = iloc_append(inst_node->code, store);
  }

  $$ = inst_node;
  if ($$ != NULL) {
    asd_tree_t *idnode = asd_new_node_from_value(&$2);
    if (idnode) idnode->type = var_type;
    asd_add_child($$, idnode);
  } else {
    free($2.value);
  }
};

optional_instantiation: TK_COM literal { 
  $$ = asd_new_unary("com", $2);
  $$->type = $2->type;
  $$->code = $2->code;
  $$->temp = $2->temp;
}
| %empty { $$ = NULL; };

literal: TK_LI_INTEIRO {
  int val = atoi($1.value);
  $$ = asd_new_node_from_value(&$1);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  $$->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, 0,0, ILOC_OP_REG, get_reg_id($$->temp));
}
| TK_LI_DECIMAL {
  int val = (int)atof($1.value);
  $$ = asd_new_node_from_value(&$1);
  $$->type = TYPE_FLOAT;
  $$->temp = iloc_new_reg();
  $$->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, 0,0, ILOC_OP_REG, get_reg_id($$->temp));
};

/* ========================================================================== */
/* ATRIBUIÇÃO                                */
/* ========================================================================== */

attribution_command: TK_ID TK_ATRIB expression {
  // CORREÇÃO CRÍTICA: Buscar na tabela ANTES de consumir $1.value
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, $1.value);
  
  if (entry == NULL) semantic_error(10, $1.line_number, $1.value); // ERR_UNDECLARED
  if (entry->nature != SYMBOL_VARIABLE) semantic_error(20, $1.line_number, $1.value); // ERR_VARIABLE
  if (entry->data_type != $3->type) semantic_error(30, $1.line_number, "Attribution type mismatch"); // ERR_WRONG_TYPE

  // Agora podemos consumir o token para criar o nó da AST
  asd_tree_t *idnode = asd_new_node_from_value(&$1);

  $$ = asd_new_binary(":=", idnode, $3);
  $$->type = entry->data_type;

  // E5: Decide base_reg (rbss ou rfp)
  int base_reg = entry->is_global ? -2 : -1;

  iloc_code_t *store = iloc_create_inst(ILOC_STOREAI,
                                        ILOC_OP_REG, get_reg_id($3->temp),
                                        ILOC_OP_REG, base_reg,
                                        ILOC_OP_INT, entry->offset);
  $$->code = iloc_append($3->code, store);
};

/* ========================================================================== */
/* CHAMADA DE FUNÇÃO                              */
/* ========================================================================== */

function_call: TK_ID '(' args ')' {
    symbol_entry_t *entry = scope_lookup(scope_stack_pointer, $1.value);
    if (entry == NULL) semantic_error(10, $1.line_number, $1.value); // ERR_UNDECLARED
    if (entry->nature != SYMBOL_FUNCTION) semantic_error(21, $1.line_number, $1.value); // ERR_FUNCTION
    
    $$ = asd_new_function_call_node(&$1, $3);
    $$->type = entry->data_type;

    if ($3) $$->code = $3->code;
    $$->temp = iloc_new_reg(); 
};

args: expression {
  $$ = $1;
}
| expression ',' args {
  $$ = asd_add_child_to_node($1, $3);
  if ($1 && $3) $$->code = iloc_append($1->code, $3->code);
}
| %empty { $$ = NULL; };

/* ========================================================================== */
/* RETORNO E CONTROLE                             */
/* ========================================================================== */

return_command: TK_RETORNA expression TK_ATRIB var_type {
  $$ = asd_new_unary("retorna", $2);
  $$->code = $2->code;
};

flow_control_command: conditional_struct { $$ = $1; }
| iterative_struct { $$ = $1; };

conditional_struct: TK_SE '(' expression ')' command_block else_block {
  $$ = asd_new_trinary("se", $3, $5, $6);
  
  char *L_true = iloc_new_label();
  char *L_false = iloc_new_label();
  char *L_saida = iloc_new_label();

  iloc_code_t *cbr = iloc_create_inst(ILOC_CBR,
                                      ILOC_OP_REG, get_reg_id($3->temp), // << USA HELPER
                                      ILOC_OP_LABEL, atoi(L_true + 1),
                                      ILOC_OP_LABEL, atoi(L_false + 1));

  iloc_code_t *l_true_node = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_true + 1), 0,0,0,0);
  iloc_code_t *l_false_node = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_false + 1), 0,0,0,0);
  iloc_code_t *l_saida_node = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_saida + 1), 0,0,0,0);
  iloc_code_t *jump_out = iloc_create_inst(ILOC_JUMPI, 0,0,0,0, ILOC_OP_LABEL, atoi(L_saida + 1));

  $$->code = $3->code;
  $$->code = iloc_append($$->code, cbr);
  $$->code = iloc_append($$->code, l_true_node);
  $$->code = iloc_append($$->code, $5->code);
  $$->code = iloc_append($$->code, jump_out);
  $$->code = iloc_append($$->code, l_false_node);
  if ($6) $$->code = iloc_append($$->code, $6->code);
  $$->code = iloc_append($$->code, l_saida_node);

  free(L_true); free(L_false); free(L_saida);
};

else_block: TK_SENAO command_block { $$ = $2; }
| %empty { $$ = NULL; };

iterative_struct: TK_ENQUANTO '(' expression ')' command_block {
  $$ = asd_new_binary("enquanto", $3, $5);

  char *L_inicio = iloc_new_label();
  char *L_true = iloc_new_label();
  char *L_saida = iloc_new_label();

  iloc_code_t *l_inicio_node = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_inicio + 1), 0,0,0,0);
  iloc_code_t *l_true_node = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_true + 1), 0,0,0,0);
  iloc_code_t *l_saida_node = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_saida + 1), 0,0,0,0);

  iloc_code_t *cbr = iloc_create_inst(ILOC_CBR,
                                      ILOC_OP_REG, get_reg_id($3->temp), // << USA HELPER
                                      ILOC_OP_LABEL, atoi(L_true + 1),
                                      ILOC_OP_LABEL, atoi(L_saida + 1));
  iloc_code_t *jump_back = iloc_create_inst(ILOC_JUMPI, 0,0,0,0, ILOC_OP_LABEL, atoi(L_inicio + 1));

  $$->code = l_inicio_node;
  $$->code = iloc_append($$->code, $3->code);
  $$->code = iloc_append($$->code, cbr);
  $$->code = iloc_append($$->code, l_true_node);
  $$->code = iloc_append($$->code, $5->code);
  $$->code = iloc_append($$->code, jump_back);
  $$->code = iloc_append($$->code, l_saida_node);

  free(L_inicio); free(L_true); free(L_saida);
};

/* ========================================================================== */
/* EXPRESSÕES                                     */
/* ========================================================================== */

expression: logical_or_expression { $$ = $1; };

logical_or_expression: logical_and_expression { $$ = $1; }
| logical_or_expression '|' logical_and_expression {
  $$ = asd_new_binary("|", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_OR, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
};

logical_and_expression: equality_expression { $$ = $1; }
| logical_and_expression '&' equality_expression {
  $$ = asd_new_binary("&", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_AND, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
};

equality_expression: relational_expression { $$ = $1; }
| equality_expression TK_OC_EQ relational_expression {
  $$ = asd_new_binary("==", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_CMP_EQ, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| equality_expression TK_OC_NE relational_expression {
  $$ = asd_new_binary("!=", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_CMP_NE, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
};

relational_expression: additive_expression { $$ = $1; }
| relational_expression '<' additive_expression {
  $$ = asd_new_binary("<", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_CMP_LT, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| relational_expression '>' additive_expression {
  $$ = asd_new_binary(">", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_CMP_GT, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| relational_expression TK_OC_LE additive_expression {
  $$ = asd_new_binary("<=", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_CMP_LE, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| relational_expression TK_OC_GE additive_expression {
  $$ = asd_new_binary(">=", $1, $3);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_CMP_GE, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
};

additive_expression: multiplicative_expression { $$ = $1; }
| additive_expression '+' multiplicative_expression {
  $$ = asd_new_binary("+", $1, $3);
  if ($1->type == $3->type) $$->type = $1->type;
  else semantic_error(30, yylineno, "Type mismatch add");
  
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_ADD, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| additive_expression '-' multiplicative_expression {
  $$ = asd_new_binary("-", $1, $3);
  if ($1->type == $3->type) $$->type = $1->type;
  else semantic_error(30, yylineno, "Type mismatch sub");
  
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_SUB, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
};

multiplicative_expression: unary_expression { $$ = $1; }
| multiplicative_expression '*' unary_expression {
  $$ = asd_new_binary("*", $1, $3);
  if ($1->type == $3->type) $$->type = $1->type;
  else semantic_error(30, yylineno, "Type mismatch mult");
  
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_MULT, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| multiplicative_expression '/' unary_expression {
  $$ = asd_new_binary("/", $1, $3);
  if ($1->type == $3->type) $$->type = $1->type;
  else semantic_error(30, yylineno, "Type mismatch div");
  
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_DIV, ILOC_OP_REG, get_reg_id($1->temp), ILOC_OP_REG, get_reg_id($3->temp), ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($1->code, $3->code);
  $$->code = iloc_append($$->code, inst);
}
| multiplicative_expression '%' unary_expression {
  $$ = asd_new_binary("%", $1, $3);
  // ILOC simples pode não ter mod, ignorado ou usar div
  $$->type = TYPE_INT; 
  $$->temp = iloc_new_reg();
  $$->code = iloc_append($1->code, $3->code);
};

unary_expression: primary_expression { $$ = $1; }
| '!' unary_expression { 
  $$ = asd_new_unary("!", $2); 
  $$->type = TYPE_INT;
  $$->code = $2->code;
  // TODO: Implementar not lógico (xori 1)
}
| '+' unary_expression { 
  $$ = asd_new_unary("+", $2); 
  $$->type = $2->type; 
  $$->code = $2->code;
  $$->temp = $2->temp;
}
| '-' unary_expression {
  $$ = asd_new_unary("-", $2);
  $$->type = $2->type;
  $$->temp = iloc_new_reg();
  iloc_code_t *inst = iloc_create_inst(ILOC_RSUBI, ILOC_OP_REG, get_reg_id($2->temp), ILOC_OP_INT, 0, ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = iloc_append($2->code, inst);
};

primary_expression: TK_ID {
  // CORREÇÃO CRÍTICA: Buscar na tabela ANTES de consumir $1.value
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, $1.value);
  if (entry == NULL) semantic_error(10, $1.line_number, $1.value);
  if (entry->nature != SYMBOL_VARIABLE) semantic_error(20, $1.line_number, $1.value);
  
  // Salvar dados importantes antes de consumir o token
  data_type_t type = entry->data_type;
  int offset = entry->offset;
  int is_global = entry->is_global;

  // Agora consome o token e libera memória
  $$ = asd_new_node_from_value(&$1);
  $$->type = type;

  $$->temp = iloc_new_reg();
  // Se for global -> rbss (-2), se local -> rfp (-1)
  int base_reg = is_global ? -2 : -1;
  
  iloc_code_t *load = iloc_create_inst(ILOC_LOADAI, ILOC_OP_REG, base_reg, ILOC_OP_INT, offset, ILOC_OP_REG, get_reg_id($$->temp));
  $$->code = load;
}
| TK_LI_INTEIRO {
  int val = atoi($1.value);
  $$ = asd_new_node_from_value(&$1);
  $$->type = TYPE_INT;
  $$->temp = iloc_new_reg();
  $$->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, 0,0, ILOC_OP_REG, get_reg_id($$->temp));
}
| TK_LI_DECIMAL {
  int val = (int)atof($1.value);
  $$ = asd_new_node_from_value(&$1);
  $$->type = TYPE_FLOAT;
  $$->temp = iloc_new_reg();
  $$->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, 0,0, ILOC_OP_REG, get_reg_id($$->temp));
}
| '(' expression ')' { $$ = $2; }
| function_call { $$ = $1; };

%%

void yyerror (char const *mensagem) {
 extern int yylineno;
 printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}
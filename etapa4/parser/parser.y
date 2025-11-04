%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asd.h"
#include "scope.h"
extern int yylineno; /* provided by Flex with %option yylineno */

void semantic_error(int error_code, int line, const char *text);

/* Arithmetic operators
 * (int, int) -> int
 * (float, float) -> float
 * (default) -> ERRO
*/

data_type_t check_arithmetic_types(data_type_t t1, data_type_t t2, int line) {
    if (t1 == TYPE_INT && t2 == TYPE_INT) return TYPE_INT;
    if (t1 == TYPE_FLOAT && t2 == TYPE_FLOAT) return TYPE_FLOAT;
    semantic_error(ERR_WRONG_TYPE, line, "mismatched types (int/float)");
    return TYPE_UNDEFINED;
}

/* Logic operators
 * (int, int) -> int
 * (default) -> ERRO
 */
data_type_t check_logical_types(data_type_t t1, data_type_t t2, int line) {
    if (t1 == TYPE_INT && t2 == TYPE_INT) return TYPE_INT;
    semantic_error(ERR_WRONG_TYPE, line, "logical operators require int operands");
    return TYPE_UNDEFINED;
}



int yylex(void);
void yyerror (char const *mensagem);
extern char *yytext;
extern asd_tree_t *arvore;
/* Global scope stack to persist across reductions */
scope_stack_t *scope_stack_pointer = NULL;
%}

%code requires {
 #include "asd.h"
 #include "lexical_value.h"
 #include "scope.h"
}
%define parse.error verbose

%union {
 asd_tree_t *node;
 lexical_value_t lexical_value;
 arg_type_node_t *arg_list; //for parameters list
 data_type_t data_type; //for "var_type"
 symbol_entry_t *symbol; //for the header to return symbol
 struct {
  data_type_t type;
  char *name;
 } param_info;
}

%type <node> program
%type <node> list
%type <node> element
%type <node> function_definition

%type <node> body command_block
%type <node> command_sequence
%type <node> simple_command
%type <node> variable_declaration
%type <node> variable_declaration_with_instantiation

%type <node> optional_instantiation
%type <node> literal
%type <node> attribution_command
%type <node> function_call
%type <node> args
%type <node> return_command
%type <node> flow_control_command
%type <node> conditional_struct
%type <node> else_block
%type <node> iterative_struct
%type <node> expression
%type <node> logical_or_expression
%type <node> logical_and_expression
%type <node> equality_expression
%type <node> relational_expression
%type <node> additive_expression
%type <node> multiplicative_expression
%type <node> unary_expression
%type <node> primary_expression

%type <node> global_escope_init
%type <node> global_escope_end

%type <node> block_scope_init

%type <data_type> var_type
%type <arg_list> optional_parameter_list parameter_list
%type <param_info> parameter 
%type <symbol> header


%token TK_TIPO "type"
%token TK_VAR "variable"
%token TK_SENAO "else"
%token TK_DECIMAL "decimal"
%token TK_SE "if"
%token TK_INTEIRO "integer"
%token TK_ATRIB ":="
%token TK_RETORNA "return"
%token TK_SETA "->"
%token TK_ENQUANTO "while"
%token TK_COM "with"
%token TK_OC_LE "<="
%token TK_OC_GE ">="
%token TK_OC_EQ "=="
%token TK_OC_NE "!="
%token <lexical_value>TK_ID "identifier"
%token <lexical_value>TK_LI_INTEIRO "integer literal"
%token <lexical_value>TK_LI_DECIMAL "decimal literal"
%token TK_ER "error"

%%

/*
  Para as ações de criação de arvore, queremos limitar a "One Liners" para melhorar a legibilidade do código
  Por isso, várias funções adicionadas ao módulo asd serão quase redundantes, em um esforço de facilitar o entendimento
  das ações que serão tomadas tanto para a criação da arvore, quanto para etapas futuras.
*/

program: global_escope_init list global_escope_end ';' {
  arvore = $2;
};

global_escope_init: %empty { 
  $$ = NULL;
  /*
  create empty table
  push table to stack
  */
  if (!scope_stack_pointer) scope_stack_pointer = scope_stack_create();
  scope_push(scope_stack_pointer, SCOPE_GLOBAL);
 };

global_escope_end: %empty {
  $$ = NULL;
/*
  pop table from stack
  free table recursively
  */
  scope_log_global_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
  scope_stack_destroy(scope_stack_pointer);
  scope_stack_pointer = NULL;
 };

/* Regra 'list' com recursão à direita (para encadeamento A -> B -> C) */
list: element {
  $$ = $1;
}
| element ',' list {
  $$ = asd_select_head_and_attach_tail($1, $3);
};

element: function_definition {
  $$ = $1;
}
| variable_declaration {
  $$ = $1;
};

function_definition: header  {

  //$1 is symbol_entry_t* returned by header
  //1. Create new function scope
  scope_push(scope_stack_pointer, SCOPE_FUNCTION);

  //2. Add parameters as LOCAL VARIABLES in this new scope
  for(arg_type_node_t *arg = $1->args; arg; arg = arg->next) {
    
    lexical_value_t param_lex;
    param_lex.line_number = $1->lexical.line_number;
    param_lex.token_type = "param";
    param_lex.value = arg->name; //do not free this

    symbol_entry_t *param_entry = scope_insert_current(scope_stack_pointer, arg->name, SYMBOL_VARIABLE, arg->type, &param_lex);

    //Verifies double declaration of parameters
    if(param_entry == NULL) {
      semantic_error(ERR_DECLARED, param_lex.line_number, arg->name);
    }
  }
} body {
  //$1 = header, $3 = body (AST node)

  //3. Create AST NODE for fuction function definition
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "function %s", $1->key);
  $$ = asd_new(buffer);

  //Assigns node function type with its return type
  $$->type = $1->data_type;

  //Appends function's body
  asd_add_child($$, $3);

  //4. Destroys function's scope
  scope_log_function_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);

};

header: TK_ID TK_SETA var_type optional_parameter_list TK_ATRIB {
  //1. Declares function in current scope
  symbol_entry_t *entry = scope_insert_current(scope_stack_pointer, $1.value, SYMBOL_FUNCTION, $3, &$1);
  
  //2. Verifies double declaration error
  if(entry == NULL) {
    semantic_error(ERR_DECLARED, $1.line_number, $1.value);
  }

  //3. Appends parameter's list to function symbol
  entry->args = $4;

  //4. Returns entry of symbol table
  $$ = entry;
  free($1.value);
};

optional_parameter_list: %empty {
  $$ = NULL;
}
| TK_COM parameter_list {
  $$ = $2;
}
| parameter_list { 
  $$ = $1;
};

parameter_list: parameter {
  //$1 is param_info
  //creates list's 1st node
  
  $$ = NULL;
  $$ = args_append($$, $1.type, $1.name);
  free($1.name);
}
| parameter_list ',' parameter {
  //$1 is arg_list
  //$3 is param_info
  //appends new parameter to the existent list

  $$ = $1;
  $$ = args_append($$, $3.type, $3.name);
  free($3.name);
};

parameter: TK_ID TK_ATRIB var_type {
  $$.type = $3;
  $$.name = $1.value;
};

body: command_block {
  $$ = $1;
};

/*
every ']' must create a new scope as well as every '[' must destroy it.
*/

command_block: block_scope_init command_sequence ']' {
  $$ = $2;
  scope_log_block_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
}
| block_scope_init ']' {
  $$ = NULL;
  scope_log_block_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer); // destroys scope's block
};

/* new rule to stack scope before processing commands */
block_scope_init: '[' {
  $$ = NULL;
  scope_push(scope_stack_pointer, SCOPE_BLOCK);
};

command_sequence: simple_command {
  $$ = $1;
}
| simple_command command_sequence {
  $$ = asd_select_head_and_attach_tail($1, $2);
};

simple_command: variable_declaration_with_instantiation {
  $$ = $1;
}
| function_call {
  $$ = $1;
}
| attribution_command {
  $$ = $1;
}
| return_command {
  $$ = $1;
}
| flow_control_command {
  $$ = $1;
}
| command_block {
  $$ = $1;
};

variable_declaration: TK_VAR TK_ID TK_ATRIB var_type {
  //const data_type_t data_type = (strcmp($4.value, "decimal") == 0) ? TYPE_FLOAT : TYPE_INT;
  
  const data_type_t data_type = $4;
    
  //Verifies declaration error
  if(scope_insert_current(scope_stack_pointer, $2.value, SYMBOL_VARIABLE, data_type, &$2) == NULL) {
    semantic_error(ERR_DECLARED, $2.line_number, $2.value);
    }

  $$ = NULL;  
  free($2.value);
};

variable_declaration_with_instantiation: TK_VAR TK_ID TK_ATRIB var_type optional_instantiation {
  
  const data_type_t var_type = $4;
  //1. Declares variable in current scope
  symbol_entry_t *entry = scope_insert_current(scope_stack_pointer,
                                                    $2.value,
                                                    SYMBOL_VARIABLE,
                                                    var_type,
                                                    &$2);

  //2. Verifies declaration error
  if(entry == NULL) {
    semantic_error(ERR_DECLARED, $2.line_number, $2.value);
  }

  asd_tree_t *instatiation_node = $5;

  //3. Verifies instantiation type
  if(instatiation_node != NULL) {
    if(instatiation_node->type != var_type) {
      semantic_error(ERR_WRONG_TYPE, $2.line_number, "variable instantiation type mismatch");
    }
  }

  //4. AST building
  $$ = instatiation_node;
  if ($$ != NULL) {
    asd_tree_t *idnode = asd_new_node_from_value(&$2);
    if(idnode) idnode->type = var_type; //Assigns ID type in AST
    asd_add_child($$, idnode);
  } else {
    free($2.value);
  }
};

var_type: TK_DECIMAL {
  $$ = TYPE_FLOAT;
}
| TK_INTEIRO {
  $$ = TYPE_INT;
};

optional_instantiation: TK_COM literal { 
  $$ = asd_new_unary("com", $2);
  if($$) $$->type = $2->type; //Propagates son's type
}
| %empty {
  $$ = NULL;
};

literal: TK_LI_INTEIRO {
  $$ = asd_new_node_from_value(&$1);
  if($$) $$->type = TYPE_INT; //Assigns type
  //appends literal on symbol table. IS IT NECESSARY? IDK
  scope_insert_current(scope_stack_pointer, $1.value, SYMBOL_LITERAL, TYPE_INT, &$1);
}
| TK_LI_DECIMAL {
  $$ = asd_new_node_from_value(&$1);
  if($$) $$->type = TYPE_FLOAT; //Assigns type
  scope_insert_current(scope_stack_pointer, $1.value, SYMBOL_LITERAL, TYPE_FLOAT, &$1);
};

attribution_command: TK_ID TK_ATRIB expression {
  //1. Verifies undeclared error
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, $1.value);
  if(entry == NULL) {
    semantic_error(ERR_UNDECLARED, $1.line_number, $1.value);
  }

  //2. Verifies function error
  if(entry->nature != SYMBOL_VARIABLE) {
    semantic_error(ERR_FUNCTION, $1.line_number, $1.value);
  }

  //3. Verifies type
  if(entry->data_type != $3->type) {
    semantic_error(ERR_WRONG_TYPE, $1.line_number, "attribution type mismatch");
  }

  asd_tree_t *idnode = asd_new_node_from_value(&$1);
  if(idnode) idnode->type = entry->data_type; //Type annotation
  $$ = asd_new_binary(":=", idnode, $3);

  if($$) $$->type = entry->data_type;
};

function_call: TK_ID '(' args ')' {
  //1. Verifies undeclared error
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, $1.value);
  if(entry == NULL) {
    semantic_error(ERR_UNDECLARED, $1.line_number, $1.value);
  }

  //2. Verifies variable error
  if(entry->nature != SYMBOL_FUNCTION) {
    semantic_error(ERR_VARIABLE, $1.line_number, $1.value);
  }

  //3. Args verification - subject 4
  arg_type_node_t *expected_arg = entry->args;
  asd_tree_t *provided_arg = $3; /* $3 é o nó 'args' */
  
  while (provided_arg && expected_arg) {
    /* Verifica o tipo [cite: 53] */
    if (provided_arg->type != expected_arg->type) {
        semantic_error(ERR_WRONG_TYPE_ARGS, $1.line_number, $1.value);
    }
    expected_arg = expected_arg->next;
    /* A sua gramática de 'args' cria uma lista encadeada via 'children' */
    provided_arg = (provided_arg->number_of_children > 0) ? provided_arg->children[0] : NULL;
  }
  /* 3. VERIFICAÇÃO DE CONTAGEM DE ARGUMENTOS */
  if (provided_arg != NULL && expected_arg == NULL) {
    /* Argumentos fornecidos a mais */
      semantic_error(ERR_EXCESS_ARGS, $1.line_number, $1.value);
    }
    if (provided_arg == NULL && expected_arg != NULL) {
      /* Argumentos faltando */
        semantic_error(ERR_MISSING_ARGS, $1.line_number, $1.value);
    }


  $$ = asd_new_function_call_node(&$1, $3);
  if($$) $$->type = entry->data_type;
};


/* Regra 'args' com recursão à direita (para encadeamento arg1 -> arg2 -> ...) */
args: expression {
  $$ = $1;
}
| expression ',' args {
  $$ = asd_add_child_to_node($1, $3);
}
| %empty {
  $$ = NULL;
};


return_command: TK_RETORNA expression TK_ATRIB var_type {
  $$ = asd_new_unary("retorna", $2);
  if($2->type != $4) {
    semantic_error(ERR_WRONG_TYPE, yylineno, "return expression type mismatch");
  }
  if($$) $$->type = $2->type;
};

flow_control_command: conditional_struct {
  $$ = $1;
}
| iterative_struct {
  $$ = $1;
}


conditional_struct: TK_SE '(' expression ')' command_block else_block {
  if($3->type != TYPE_INT) {
    semantic_error(ERR_WRONG_TYPE, yylineno, "if test expression must be int");
  }
  
  
  $$ = asd_new_trinary("se", $3, $5, $6);
  if($$) $$->type = $3->type;
};

else_block: TK_SENAO command_block { 
  $$ = $2; 
}
| %empty {
  $$ = NULL;
};

iterative_struct: TK_ENQUANTO '(' expression ')' command_block {
  if($3->type != TYPE_INT) {
    semantic_error(ERR_WRONG_TYPE, yylineno, "while test expression must be int");
  }
  $$ = asd_new_binary("enquanto", $3, $5);
  if($$) $$->type = $3->type;
};

//EXPRESSOES

expression: logical_or_expression {
  $$ = $1;
};

logical_or_expression: logical_and_expression {
  $$ = $1;
}
| logical_or_expression '|' logical_and_expression {
  $$ = asd_new_binary("|", $1, $3);
  $$->type = check_logical_types($1->type, $3->type, yylineno);
};

logical_and_expression: equality_expression {
  $$ = $1;
}
| logical_and_expression '&' equality_expression {
  $$ = asd_new_binary("&", $1, $3);
  $$->type = check_logical_types($1->type, $3->type, yylineno);
};

equality_expression: relational_expression {
  $$ = $1;
}
| equality_expression TK_OC_EQ relational_expression {
  $$ = asd_new_binary("==", $1, $3);
  check_arithmetic_types($1->type, $3->type, yylineno);
  $$->type = TYPE_INT;
}
| equality_expression TK_OC_NE relational_expression {
  $$ = asd_new_binary("!=", $1, $3);
  check_arithmetic_types($1->type, $3->type, yylineno);
  $$->type = TYPE_INT;
};

relational_expression: additive_expression {
  $$ = $1;
}
| relational_expression '<' additive_expression {
  $$ = asd_new_binary("<", $1, $3);
  check_arithmetic_types($1->type, $3->type, yylineno);
  $$->type = TYPE_INT;
  }
| relational_expression '>' additive_expression {
  $$ = asd_new_binary(">", $1, $3);
  check_arithmetic_types($1->type, $3->type, yylineno);
  $$->type = TYPE_INT;
}
| relational_expression TK_OC_LE additive_expression {
  $$ = asd_new_binary("<=", $1, $3);
  check_arithmetic_types($1->type, $3->type, yylineno);
  $$->type = TYPE_INT;
}
| relational_expression TK_OC_GE additive_expression {
  $$ = asd_new_binary(">=", $1, $3);
  check_arithmetic_types($1->type, $3->type, yylineno);
  $$->type = TYPE_INT;
};

additive_expression: multiplicative_expression {
  $$ = $1;
}
| additive_expression '+' multiplicative_expression {
  $$ = asd_new_binary("+", $1, $3);
  $$->type = check_arithmetic_types($1->type, $3->type, yylineno);
}
| additive_expression '-' multiplicative_expression {
  $$ = asd_new_binary("-", $1, $3);
  $$->type = check_arithmetic_types($1->type, $3->type, yylineno);
};

multiplicative_expression: unary_expression {
  $$ = $1;
}
| multiplicative_expression '*' unary_expression {
  $$ = asd_new_binary("*", $1, $3);
  $$->type = check_arithmetic_types($1->type, $3->type, yylineno);
}
| multiplicative_expression '/' unary_expression {
  $$ = asd_new_binary("/", $1, $3);
  $$->type = check_arithmetic_types($1->type, $3->type, yylineno);
}
| multiplicative_expression '%' unary_expression {
  $$ = asd_new_binary("%", $1, $3);

  if($1->type != TYPE_INT || $3->type != TYPE_INT) {
    semantic_error(ERR_WRONG_TYPE, yylineno, "modulo (%) operator requires int operator");
  }
  $$->type = TYPE_INT;
};

unary_expression: primary_expression {
  $$ = $1;
}
| '!' unary_expression { 
  $$ = asd_new_unary("!", $2);
  //'!' only works with int (boolean) and returns an int
  if($2->type != TYPE_INT) {
    semantic_error(ERR_WRONG_TYPE, yylineno, "logic NOT ('!') requires int operand");
  }


}
| '+' unary_expression { 
  $$ = asd_new_unary("+", $2);
  $$->type = $2->type;
}
| '-' unary_expression { 
  $$ = asd_new_unary("-", $2);
  $$->type = $2->type;
};

primary_expression: TK_ID {
  
  //1. Verifies undeclared error
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, $1.value);
  if(entry == NULL) {
    semantic_error(ERR_UNDECLARED, $1.line_number, $1.value);
  }
  
  //2. Verifies fuction error
  if(entry->nature != SYMBOL_VARIABLE) {
    semantic_error(ERR_FUNCTION, $1.line_number, $1.value);
  }

  $$ = asd_new_node_from_value(&$1);

  //Type annotation - informs to AST the node type
  if($$) $$->type = entry->data_type;

}
| TK_LI_INTEIRO {
  $$ = asd_new_node_from_value(&$1);
  if($$) $$->type = TYPE_INT; //Type annotation
  scope_insert_current(scope_stack_pointer, $1.value, SYMBOL_LITERAL, TYPE_INT, &$1);
}
| TK_LI_DECIMAL {
  $$ = asd_new_node_from_value(&$1);
  if($$) $$->type = TYPE_FLOAT; //Type annotation
  scope_insert_current(scope_stack_pointer, $1.value, SYMBOL_LITERAL, TYPE_FLOAT, &$1);
}
| '(' expression ')' { $$ = $2; }
| function_call { $$ = $1; };

%%

void yyerror (char const *mensagem) {
 extern int yylineno; // Declare yylineno to access the current line number
 printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}

void semantic_error(int error_code, int line, const char *text) {
  fprintf(stderr, "Semantic ERROR at line %d: %s (Code: %d)\n", line, text, error_code);
  exit(error_code); // O PDF exige que o programa saia imediatamente [cite: 67]
}
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asd.h"
int yylex(void);
void yyerror (char const *mensagem);
extern char *yytext;
extern asd_tree_t *arvore;
%}

%code requires {
 #include "asd.h"
 #include "lexical_value.h"
}
%define parse.error verbose

%union {
 asd_tree_t *node;
 lexical_value_t lexical_value;
}

%type <node> program
%type <node> list
%type <node> element
%type <node> function_definition
%type <node> header
%type <node> optional_parameter_list
%type <node> parameter_list
%type <node> parameter
%type <node> body command_block
%type <node> command_sequence
%type <node> simple_command
%type <node> variable_declaration
%type <node> variable_declaration_with_instantiation
%type <node> var_type
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
%type <node> escope_init
%type <node> escope_end

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

program: escope_init list escope_end ';' {
  arvore = $2;
};

escope_init: %empty { 
  $$ = NULL;
  /*
  create empty table
  push table to stack
  */
 };

escope_end: %empty {
  $$ = NULL;
/*
  pop table from stack
  free table recursively
  */
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

function_definition: header body {
  $$ = asd_add_child_to_node($1, $2);
};

header: TK_ID TK_SETA var_type optional_parameter_list TK_ATRIB {
  $$ = asd_new_node_from_value(&$1);
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
  $$ = NULL; 
}
| parameter_list ',' parameter {
  $$ = NULL;
};

parameter: TK_ID TK_ATRIB var_type {
  $$ = NULL; free($1.value);
};

body: command_block {
  $$ = $1;
};

command_block: '[' command_sequence ']' {
  $$ = $2;
}
| '[' ']' {
  $$ = NULL;
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
  $$ = NULL;  free($2.value);
};

variable_declaration_with_instantiation: TK_VAR TK_ID TK_ATRIB var_type optional_instantiation {
  // Only rule that is difficult to reduce
  $$ = $5;
  if ($$ != NULL) {
    asd_tree_t *idnode = asd_new_node_from_value(&$2);
    asd_add_child($$, idnode);
  } else {
    free($2.value);
  }
};

var_type: TK_DECIMAL {
  $$ = NULL;
}
| TK_INTEIRO {
  $$ = NULL;
};

optional_instantiation: TK_COM literal { 
  $$ = asd_new_unary("com", $2);
}
| %empty {
  $$ = NULL;
};

literal: TK_LI_INTEIRO {
  $$ = asd_new_node_from_value(&$1);
}
| TK_LI_DECIMAL {
  $$ = asd_new_node_from_value(&$1);
};

attribution_command: TK_ID TK_ATRIB expression {
  asd_tree_t *idnode = asd_new_node_from_value(&$1);
  $$ = asd_new_binary(":=", idnode, $3);
};

function_call: TK_ID '(' args ')' {
    $$ = asd_new_function_call_node(&$1, $3);
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
};

flow_control_command: conditional_struct {
  $$ = $1;
}
| iterative_struct {
  $$ = $1;
}


conditional_struct: TK_SE '(' expression ')' command_block else_block {
  $$ = asd_new_trinary("se", $3, $5, $6);
};

else_block: TK_SENAO command_block { 
  $$ = $2; 
}
| %empty {
  $$ = NULL;
};

iterative_struct: TK_ENQUANTO '(' expression ')' command_block {
  $$ = asd_new_binary("enquanto", $3, $5);
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
};

logical_and_expression: equality_expression {
  $$ = $1;
}
| logical_and_expression '&' equality_expression {
  $$ = asd_new_binary("&", $1, $3);
};

equality_expression: relational_expression {
  $$ = $1;
}
| equality_expression TK_OC_EQ relational_expression {
  $$ = asd_new_binary("==", $1, $3);
}
| equality_expression TK_OC_NE relational_expression {
  $$ = asd_new_binary("!=", $1, $3);
};

relational_expression: additive_expression {
  $$ = $1;
}
| relational_expression '<' additive_expression {
  $$ = asd_new_binary("<", $1, $3);
}
| relational_expression '>' additive_expression {
  $$ = asd_new_binary(">", $1, $3);
}
| relational_expression TK_OC_LE additive_expression {
  $$ = asd_new_binary("<=", $1, $3);
}
| relational_expression TK_OC_GE additive_expression {
  $$ = asd_new_binary(">=", $1, $3);
};

additive_expression: multiplicative_expression {
  $$ = $1;
}
| additive_expression '+' multiplicative_expression {
  $$ = asd_new_binary("+", $1, $3);
}
| additive_expression '-' multiplicative_expression {
  $$ = asd_new_binary("-", $1, $3);
};

multiplicative_expression: unary_expression {
  $$ = $1;
}
| multiplicative_expression '*' unary_expression {
  $$ = asd_new_binary("*", $1, $3);
}
| multiplicative_expression '/' unary_expression {
  $$ = asd_new_binary("/", $1, $3);
}
| multiplicative_expression '%' unary_expression {
  $$ = asd_new_binary("%", $1, $3);
};

unary_expression: primary_expression {
  $$ = $1;
}
| '!' unary_expression { 
  $$ = asd_new_unary("!", $2);
}
| '+' unary_expression { 
  $$ = asd_new_unary("+", $2);
}
| '-' unary_expression { 
  $$ = asd_new_unary("-", $2);
};

primary_expression: TK_ID {
  $$ = asd_new_node_from_value(&$1);
}
| TK_LI_INTEIRO {
  $$ = asd_new_node_from_value(&$1);
}
| TK_LI_DECIMAL {
  $$ = asd_new_node_from_value(&$1);
}
| '(' expression ')' { $$ = $2; }
| function_call { $$ = $1; };

%%

void yyerror (char const *mensagem) {
 extern int yylineno; // Declare yylineno to access the current line number
 printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}
%{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "asd.h"
int yylex(void);
void yyerror (char const *mensagem);
extern char *yytext;
extern asd_tree_t *arvore;
%}

%code requires {
  #include "asd.h"
  #include "parser.tab.h"

  // Declaring lexical_value in here makes it accessible in .h
  typedef struct {
    int line_number;
    char *token_type;
    char *value;
  } lexical_value_t;
}
%define parse.error verbose

%union {
  asd_tree_t *node;
  lexical_value_t lexical_value;
}

%type <node> program list element function_definition header optional_parameter_list parameter_list parameter body command_block command_sequence simple_command variable_declaration variable_declaration_with_instantiation var_type optional_instantiation literal attribution_command function_call args return_command flow_control_command conditional_struct else_block iterative_struct expression logical_or_expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression primary_expression

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

program: list ';' {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);

  // Set arvore to point to the root of the tree
  arvore = $$;
}
| %empty {
  $$ = asd_new("empty program");
  arvore = $$;
};

list: element {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| list ',' element {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

element: function_definition {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| variable_declaration {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
};

function_definition: header body {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
  asd_add_child($$, $2);
};

header: "identifier" "->" var_type optional_parameter_list ":=" {
  $$ = asd_new("header");
  asd_add_child($$, $3);
};

optional_parameter_list: parameter_list {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| "with" parameter_list {
  $$ = asd_new($2->label);
  asd_add_child($$, $2);
}
| %empty {
  $$ = asd_new("empty param list");
};

parameter_list: parameter {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| parameter_list ',' parameter {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

parameter: "identifier" ":=" var_type {
  $$ = asd_new("parameter");
  asd_add_child($$, $3);
};

body: command_block {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
};

command_block: '[' command_sequence ']' {
  $$ = asd_new($2->label);
  asd_add_child($$, $2);
}
| '[' ']' {
$$ = asd_new("empty command block");
};

command_sequence: simple_command {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| command_sequence simple_command {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
  asd_add_child($$, $2);
};

simple_command: variable_declaration_with_instantiation {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| function_call {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| attribution_command {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| return_command {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| flow_control_command {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
};

variable_declaration: "variable" "identifier" ":=" var_type {
  // this labels are just to see the tree in the output. Probably are wrong
  $$ = asd_new("declare");
  asd_add_child($$, $4);
};

variable_declaration_with_instantiation: "variable" "identifier" ":=" var_type optional_instantiation {
    $$ = asd_new("declare with");
    asd_add_child($$, $4);
    asd_add_child($$, $5);
};

var_type: "decimal" {
  $$ = asd_new("decimal");
}
         | "integer" {
  $$ = asd_new("integer");
};

optional_instantiation: "with" literal {
  $$ = asd_new("with");
  asd_add_child($$, $2);
}
| %empty {
  $$ = asd_new("empty with");
};

literal: "integer literal" {
  $$ = asd_new($1.value);
}
| "decimal literal" {
  $$ = asd_new($1.value);
};

attribution_command: "identifier" ":=" expression {
  char buffer[256]; // Adjust size as needed
  sprintf(buffer, "%s := %s", $1.value, $3->label);
  $$ = asd_new(buffer);
  // two children, the identifier and the expression.
  asd_add_child($$, $3);
};

function_call: "identifier" '(' args ')' {
  // Only used to concatenate function name with 'call' 
  char buffer[256]; // Adjust size as needed
  sprintf(buffer, "call %s", $1.value);
  $$ = asd_new(buffer);
  asd_add_child($$, $3);
};

args: %empty {
  // for now creating node for args and leaving it without children as the else block.
  $$ = asd_new("empty args");
}
| args ',' expression {
  $$ = asd_new("args");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| expression {
  $$ = asd_new("args");
  asd_add_child($$, $1);
};

return_command: "return" expression ":=" var_type {
  $$ = asd_new("return");
  // return has only one child that is the expression.
  asd_add_child($$, $2);
};

flow_control_command: conditional_struct {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| iterative_struct {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
};

conditional_struct: "if" '(' expression ')' command_block else_block {
  $$ = asd_new("if");
  asd_add_child($$, $3);
  asd_add_child($$, $5);
  asd_add_child($$, $6);
};

else_block: %empty {
  // maybe this should be empty, for now just putting the else node with zero or one children. and son of if node.
  $$ = asd_new("empty else");
}
| "else" command_block {
  $$ = asd_new("else");
  asd_add_child($$, $2);
};

iterative_struct: "while" '(' expression ')' command_block {
  // maybe the label here should be just 'enquanto'
  $$ = asd_new("while");
  // two children, the expression and the command block
  asd_add_child($$, $3);
  asd_add_child($$, $5);
};

expression: logical_or_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
};

logical_or_expression: logical_and_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| logical_or_expression '|' logical_and_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

logical_and_expression: equality_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| logical_and_expression '&' equality_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

equality_expression: relational_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| equality_expression "==" relational_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| equality_expression "!=" relational_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

relational_expression: additive_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| relational_expression '<' additive_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression '>' additive_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression "<=" additive_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression ">=" additive_expression {
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

additive_expression: multiplicative_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| additive_expression '+' multiplicative_expression {
  $$ = asd_new("+");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| additive_expression '-' multiplicative_expression {
  $$ = asd_new("-");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

multiplicative_expression: unary_expression { 
  $$ = asd_new($1->label);
  asd_add_child($$, $1); 
}
| multiplicative_expression '*' unary_expression {
  $$ = asd_new("*");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| multiplicative_expression '/' unary_expression {
  $$ = asd_new("/");
  asd_add_child($$, $1);
  asd_add_child($$, $3); 
}
| multiplicative_expression '%' unary_expression {
  $$ = asd_new("%");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

unary_expression: primary_expression { 
    $$ = asd_new($1->label); 
    asd_add_child($$, $1); 
}
| '+' unary_expression { 
    $$ = asd_new("+"); 
    asd_add_child($$, $2); 
}
| '-' unary_expression { 
    $$ = asd_new("-"); 
    asd_add_child($$, $2); 
}
| '!' unary_expression { 
    $$ = asd_new("!"); 
    asd_add_child($$, $2); 
};

primary_expression: "identifier" { 
    $$ = asd_new($1.value); 
}
| "integer literal" { 
    $$ = asd_new($1.value); 
}
| "decimal literal" { 
    $$ = asd_new($1.value); 
}
| '(' expression ')' { 
    $$ = asd_new($2->label); 
    asd_add_child($$, $2); 
}
| function_call { 
    $$ = asd_new($1->label); 
    asd_add_child($$, $1); 
};
%%

void yyerror (char const *mensagem) {
  extern int yylineno; // Declare yylineno to access the current line number
  printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}
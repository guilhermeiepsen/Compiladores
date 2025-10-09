%{
#include <math.h>
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror (char const *mensagem);
extern char *yytext;
// Define the struct for lexical values
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

%type <node> program list element function_definition header optional_parameter_list parameter_list parameter body command_block command_sequence simple_command variable_declaration variable_declaration_with_instantiation var_type optional_instantiation literal attribution_command function_call args return_command flow_control_command conditional_struct else_block mandatory_block iterative_struct expression logical_or_expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression primary_expression

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

program: list ';'
       | %empty;

list: element
    | list ',' element;

element: function_definition
       | variable_declaration;

function_definition: header body;

header: "identifier" "->" var_type optional_parameter_list ":=";

optional_parameter_list: parameter_list
                       | "with" parameter_list
                       | %empty;

parameter_list: parameter
              | parameter_list ',' parameter;

parameter: "identifier" ":=" var_type;

body: command_block;

command_block: '[' command_sequence ']'
             | '[' ']';

command_sequence: simple_command
                | command_sequence simple_command;

simple_command: variable_declaration_with_instantiation
              | function_call
              | attribution_command
              | return_command
              | flow_control_command;

variable_declaration: "variable" "identifier" ":=" var_type;

variable_declaration_with_instantiation: "variable" "identifier" ":=" var_type optional_instantiation;

var_type: "decimal"
         | "integer";

optional_instantiation: "with" literal
                       | %empty;

literal: "integer literal"
        | "decimal literal";

attribution_command: "identifier" ":=" expression;

function_call: "identifier" '(' args ')';

args: %empty;
args: args ',' expression
    | expression;

return_command: "return" expression ":=" var_type;

flow_control_command: conditional_struct
                     | iterative_struct;

conditional_struct: "if" '(' expression ')' '[' mandatory_block ']' else_block;

else_block: %empty
          | "else" command_block;

mandatory_block: mandatory_block simple_command
               | simple_command;

iterative_struct: "while" '(' expression ')' command_block {
  // maybe the label here should be just 'enquanto'
  $$ = asd_new("while" + "(" + $2->label + ")");
  // two children, the expression and the command block
  asd_add_child($$, $2);
  asd_add_child($$, $3);
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
  printf("YYTEXT ON LOGICAL OR EXPRESSION | %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

logical_and_expression: equality_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| logical_and_expression '&' equality_expression {
  printf("YYTEXT ON LOGICAL AND EXPRESSION & %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

equality_expression: relational_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| equality_expression "==" relational_expression {
  printf("YYTEXT ON EQUALITY EXPRESSION == %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| equality_expression "!=" relational_expression {
  printf("YYTEXT ON EQUALITY EXPRESSION != %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

relational_expression: additive_expression {
  $$ = asd_new($1->label);
  asd_add_child($$, $1);
}
| relational_expression '<' additive_expression {
  printf("YYTEXT ON RELATIONAL EXPRESSION < %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression '>' additive_expression {
  printf("YYTEXT ON RELATIONAL EXPRESSION > %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression "<=" additive_expression {
  printf("YYTEXT ON RELATIONAL EXPRESSION <= %s\n", yytext);
  $$ = asd_new(yytext);
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression ">=" additive_expression {
  printf("YYTEXT ON RELATIONAL EXPRESSION >= %s\n", yytext);
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
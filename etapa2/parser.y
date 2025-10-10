%{
#include <stdio.h>
int yylex(void);
void yyerror (char const *mensagem);
%}
%define parse.error verbose

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
%token TK_ID "identifier"
%token TK_LI_INTEIRO "integer literal"
%token TK_LI_DECIMAL "decimal literal"
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

iterative_struct: "while" '(' expression ')' command_block;

expression: logical_or_expression;

logical_or_expression: logical_and_expression
                     | logical_or_expression '|' logical_and_expression;

logical_and_expression: equality_expression
                      | logical_and_expression '&' equality_expression;

equality_expression: relational_expression
                    | equality_expression "==" relational_expression
                    | equality_expression "!=" relational_expression;

relational_expression: additive_expression
                     | relational_expression '<' additive_expression
                     | relational_expression '>' additive_expression
                     | relational_expression "<=" additive_expression
                     | relational_expression ">=" additive_expression;

additive_expression: multiplicative_expression
                   | additive_expression '+' multiplicative_expression
                   | additive_expression '-' multiplicative_expression;

multiplicative_expression: unary_expression
                         | multiplicative_expression '*' unary_expression
                         | multiplicative_expression '/' unary_expression
                         | multiplicative_expression '%' unary_expression;

unary_expression: primary_expression
                | '+' unary_expression
                | '-' unary_expression
                | '!' unary_expression;

primary_expression: "identifier"
                  | "integer literal"
                  | "decimal literal"
                  | '(' expression ')'
                  | function_call;


%%

void yyerror (char const *mensagem) {
  extern int yylineno; // Declare yylineno to access the current line number
  printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}
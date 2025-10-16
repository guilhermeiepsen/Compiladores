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

program:
  list ';'
  {
    arvore = $1;
  }
;


/* Regra 'list' com recursão à direita (para encadeamento A -> B -> C) */
list:
    element
    {
        $$ = $1;
    }
|   element ',' list
    {
        /* Anexa o resto da lista ($3) como filho do elemento atual ($1) */
        if ($1 != NULL) {
            if ($3 != NULL) asd_add_child($1, $3);
            $$ = $1;
        } else {
            /* Se o elemento atual for nulo (uma declaração global), a lista começa do próximo */
            $$ = $3;
        }
    }
;



element: function_definition { $$ = $1; }
| variable_declaration { $$ = $1; };

function_definition: header body {
    $$ = $1;
    if ($2) asd_add_child($$, $2); // Adiciona o corpo diretamente
};

/* ALTERADO: Header simplificado para remover tipo de retorno e sempre desempacotar parâmetros */
header: TK_ID TK_SETA var_type optional_parameter_list TK_ATRIB {
  $$ = asd_new($1.value);    /* nome da função */
  free($1.value);
};

/* ALTERADO: Regra simplificada para sempre passar a lista para o header desempacotar */
optional_parameter_list:
  %empty {}
| TK_COM parameter_list {}
| parameter_list {}
;

parameter_list:
  parameter {}
| parameter_list ',' parameter {}
;

parameter: TK_ID TK_ATRIB var_type { free($1.value);};

body: command_block { $$ = $1; };

command_block: '[' command_sequence ']' { $$ = $2; }
| '[' ']' {  };

command_sequence: simple_command
    {
        $$ = $1;
    }
|  simple_command command_sequence
    {
        if ($1 != NULL) { // Se o comando atual não for uma declaração nula
            if ($2 != NULL) asd_add_child($1, $2); // Anexa o resto da corrente
            $$ = $1;
        } else {
            $$ = $2; // Se o comando for nulo, a corrente começa do próximo
        }
    }
;

simple_command: variable_declaration_with_instantiation { $$ = $1; }
| function_call { $$ = $1; }
| attribution_command { $$ = $1; }
| return_command { $$ = $1; }
| flow_control_command { $$ = $1; };

/* ALTERADO: Regras de declaração agora retornam NULL para serem ignoradas na árvore */
variable_declaration: TK_VAR TK_ID TK_ATRIB var_type {
  $$ = NULL;
  free($2.value);
};

variable_declaration_with_instantiation: TK_VAR TK_ID TK_ATRIB var_type optional_instantiation {
  $$ = $5;
  if ($$ != NULL) {
    asd_tree_t *idnode = asd_new($2.value);
    asd_add_child($$, idnode);
  }

  free($2.value);
};

var_type: TK_DECIMAL { }
| TK_INTEIRO { };

optional_instantiation: TK_COM literal { 
  $$ = asd_new("com");
  asd_add_child($$, $2);
}
| %empty { $$ = NULL; };

literal: TK_LI_INTEIRO { $$ = asd_new($1.value);
 free($1.value);
 }
| TK_LI_DECIMAL { $$ = asd_new($1.value);
 free($1.value);
 };

attribution_command: TK_ID TK_ATRIB expression {
  $$ = asd_new(":=");
  asd_tree_t *idnode = asd_new($1.value);
  free($1.value);
  asd_add_child($$, idnode);
  asd_add_child($$, $3);
};

/* Regra 'function_call' simplificada */
function_call: TK_ID '(' args ')' {
    char buffer[256];
    sprintf(buffer, "call %s", $1.value);
    $$ = asd_new(buffer);
    free($1.value);
    if ($3) asd_add_child($$, $3); // Adiciona a raiz da corrente de argumentos
};



/* Regra 'args' com recursão à direita (para encadeamento arg1 -> arg2 -> ...) */
args:
    expression
    {
        $$ = $1;
    }
|   expression ',' args
    {
        asd_add_child($1, $3);
        $$ = $1;
    }
|   %empty
    {
        $$ = NULL;
    }
;



return_command: TK_RETORNA expression TK_ATRIB var_type {
  $$ = asd_new("retorna");
  asd_add_child($$, $2);
};

flow_control_command: conditional_struct { $$ = $1; }
| iterative_struct { $$ = $1; };


conditional_struct: TK_SE '(' expression ')' command_block else_block {
    $$ = asd_new("se");
    asd_add_child($$, $3);
    if ($5) asd_add_child($$, $5); // Adiciona o bloco 'then' diretamente
    if ($6) asd_add_child($$, $6); // Adiciona o bloco 'else' diretamente
};

else_block: TK_SENAO command_block {
    $$ = $2; // Apenas repassa o resultado de command_block
}
| %empty { $$ = NULL; };

iterative_struct: TK_ENQUANTO '(' expression ')' command_block {
    $$ = asd_new("enquanto");
    asd_add_child($$, $3);
    if ($5) asd_add_child($$, $5); // Adiciona o corpo diretamente
};

//EXPRESSOES

expression: logical_or_expression { $$ = $1; };

logical_or_expression: logical_and_expression { $$ = $1; }
| logical_or_expression '|' logical_and_expression {
  $$ = asd_new("|");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

logical_and_expression: equality_expression { $$ = $1; }
| logical_and_expression '&' equality_expression {
  $$ = asd_new("&");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

equality_expression: relational_expression { $$ = $1; }
| equality_expression TK_OC_EQ relational_expression {
  $$ = asd_new("==");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| equality_expression TK_OC_NE relational_expression {
  $$ = asd_new("!=");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

relational_expression: additive_expression { $$ = $1; }
| relational_expression '<' additive_expression {
  $$ = asd_new("<");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression '>' additive_expression {
  $$ = asd_new(">");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression TK_OC_LE additive_expression {
  $$ = asd_new("<=");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
}
| relational_expression TK_OC_GE additive_expression {
  $$ = asd_new(">=");
  asd_add_child($$, $1);
  asd_add_child($$, $3);
};

additive_expression: multiplicative_expression { $$ = $1; }
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

multiplicative_expression: unary_expression { $$ = $1; }
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

unary_expression:
    primary_expression { $$ = $1; }
|   '!' unary_expression { 
        $$ = asd_new("!");
        asd_add_child($$, $2); 
    }
|   '+' unary_expression { 
        $$ = asd_new("+");
        asd_add_child($$, $2); 
    }
|   '-' unary_expression { 
        $$ = asd_new("-");
        asd_add_child($$, $2); 
    }
;

primary_expression: TK_ID {
  $$ = asd_new($1.value);
  free($1.value);
}
| TK_LI_INTEIRO { $$ = asd_new($1.value); 
  free($1.value);
}
| TK_LI_DECIMAL { $$ = asd_new($1.value); 
  free($1.value);
}
| '(' expression ')' { $$ = $2; }
| function_call { $$ = $1; };

%%

void yyerror (char const *mensagem) {
 extern int yylineno; // Declare yylineno to access the current line number
 printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}
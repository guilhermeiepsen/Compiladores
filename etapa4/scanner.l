/* Eu sou parte do grupo Bruno e Guilherme */
%{
  /*
  Incluir os gerais aqui, antes dos específicos nossos
  NUNCA FAZER INCLUDE DE .c
  */
  #include "parser.tab.h"
  #include <string.h>

  static void set_lexical_value(const char *kind) {
    yylval.lexical_value.line_number = yylineno;
    yylval.lexical_value.token_type = kind;
    yylval.lexical_value.value = strdup(yytext);
  }

%}

%option noyywrap
%option yylineno

%%

tipo { return TK_TIPO; }
var { return TK_VAR; }
senao { return TK_SENAO; }
decimal { return TK_DECIMAL; }
se { return TK_SE; }
inteiro { return TK_INTEIRO; }
retorna { return TK_RETORNA; }
enquanto { return TK_ENQUANTO; }
com { return TK_COM; }
[!*/%+\-<>\[\]()=,;&|] { return (int) yytext[0]; }
[a-z_][a-z0-9_]* { set_lexical_value("identifier"); return TK_ID; }
[0-9]+ { set_lexical_value("integer literal"); return TK_LI_INTEIRO; }
[0-9]*\.[0-9]+ { set_lexical_value("decimal literal"); return TK_LI_DECIMAL; }
"==" { return TK_OC_EQ; }
"<=" { return TK_OC_LE; }
">=" { return TK_OC_GE; }
"!=" { return TK_OC_NE; }
":=" { return TK_ATRIB; }
"->" { return TK_SETA; }
"//"[^\n]* { }
\n { ; }
[ \t\r\f\v]+   { }
. { return TK_ER; }

%%

int get_line_number() {
  return yylineno;
}
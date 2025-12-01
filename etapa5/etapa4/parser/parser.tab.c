/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser/parser.y"

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
data_type_t current_function_return_type = TYPE_UNDEFINED;

/* Helper function to map error codes to strings */
const char* get_error_string(int error_code) {
    switch(error_code) {
        case 10: return "ERR_UNDECLARED";
        case 11: return "ERR_DECLARED";
        case 20: return "ERR_VARIABLE";
        case 21: return "ERR_FUNCTION";
        case 30: return "ERR_WRONG_TYPE";
        case 40: return "ERR_MISSING_ARGS";
        case 41: return "ERR_EXCESS_ARGS";
        case 42: return "ERR_WRONG_TYPE_ARGS";
        default: return "UNKNOWN_ERROR";
    }
}

void semantic_error(int error_code, int line, const char *text) {
    const char* error_str = get_error_string(error_code);
    fprintf(stderr, "Semantic ERROR at line %d: %s (Code: %d - %s)\n", 
           line, text ? text : "(null)", error_code, error_str);
    exit(error_code);
}

static int get_reg_id(const char *temp_str) {
    if (!temp_str) {
        fprintf(stderr, "FATAL ERROR: get_reg_id called with NULL pointer.\n");
        exit(1);
    }
    if (temp_str[0] != 'r') {
        fprintf(stderr, "FATAL ERROR: Invalid register format: '%s'\n", temp_str);
        exit(1);
    }
    return atoi(temp_str + 1);
}

#line 123 "parser/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TK_TIPO = 3,                    /* TK_TIPO  */
  YYSYMBOL_TK_VAR = 4,                     /* TK_VAR  */
  YYSYMBOL_TK_SENAO = 5,                   /* TK_SENAO  */
  YYSYMBOL_TK_DECIMAL = 6,                 /* TK_DECIMAL  */
  YYSYMBOL_TK_SE = 7,                      /* TK_SE  */
  YYSYMBOL_TK_INTEIRO = 8,                 /* TK_INTEIRO  */
  YYSYMBOL_TK_ATRIB = 9,                   /* TK_ATRIB  */
  YYSYMBOL_TK_RETORNA = 10,                /* TK_RETORNA  */
  YYSYMBOL_TK_SETA = 11,                   /* TK_SETA  */
  YYSYMBOL_TK_ENQUANTO = 12,               /* TK_ENQUANTO  */
  YYSYMBOL_TK_COM = 13,                    /* TK_COM  */
  YYSYMBOL_TK_OC_LE = 14,                  /* TK_OC_LE  */
  YYSYMBOL_TK_OC_GE = 15,                  /* TK_OC_GE  */
  YYSYMBOL_TK_OC_EQ = 16,                  /* TK_OC_EQ  */
  YYSYMBOL_TK_OC_NE = 17,                  /* TK_OC_NE  */
  YYSYMBOL_TK_ID = 18,                     /* TK_ID  */
  YYSYMBOL_TK_LI_INTEIRO = 19,             /* TK_LI_INTEIRO  */
  YYSYMBOL_TK_LI_DECIMAL = 20,             /* TK_LI_DECIMAL  */
  YYSYMBOL_TK_ER = 21,                     /* TK_ER  */
  YYSYMBOL_22_ = 22,                       /* ';'  */
  YYSYMBOL_23_ = 23,                       /* ','  */
  YYSYMBOL_24_ = 24,                       /* ']'  */
  YYSYMBOL_25_ = 25,                       /* '['  */
  YYSYMBOL_26_ = 26,                       /* '('  */
  YYSYMBOL_27_ = 27,                       /* ')'  */
  YYSYMBOL_28_ = 28,                       /* '|'  */
  YYSYMBOL_29_ = 29,                       /* '&'  */
  YYSYMBOL_30_ = 30,                       /* '<'  */
  YYSYMBOL_31_ = 31,                       /* '>'  */
  YYSYMBOL_32_ = 32,                       /* '+'  */
  YYSYMBOL_33_ = 33,                       /* '-'  */
  YYSYMBOL_34_ = 34,                       /* '*'  */
  YYSYMBOL_35_ = 35,                       /* '/'  */
  YYSYMBOL_36_ = 36,                       /* '%'  */
  YYSYMBOL_37_ = 37,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 38,                  /* $accept  */
  YYSYMBOL_program = 39,                   /* program  */
  YYSYMBOL_global_escope_init = 40,        /* global_escope_init  */
  YYSYMBOL_global_escope_end = 41,         /* global_escope_end  */
  YYSYMBOL_list = 42,                      /* list  */
  YYSYMBOL_element = 43,                   /* element  */
  YYSYMBOL_function_definition = 44,       /* function_definition  */
  YYSYMBOL_45_1 = 45,                      /* $@1  */
  YYSYMBOL_header = 46,                    /* header  */
  YYSYMBOL_var_type = 47,                  /* var_type  */
  YYSYMBOL_optional_parameter_list = 48,   /* optional_parameter_list  */
  YYSYMBOL_parameter_list = 49,            /* parameter_list  */
  YYSYMBOL_parameter = 50,                 /* parameter  */
  YYSYMBOL_body = 51,                      /* body  */
  YYSYMBOL_command_block = 52,             /* command_block  */
  YYSYMBOL_block_scope_init = 53,          /* block_scope_init  */
  YYSYMBOL_command_sequence = 54,          /* command_sequence  */
  YYSYMBOL_simple_command = 55,            /* simple_command  */
  YYSYMBOL_variable_declaration = 56,      /* variable_declaration  */
  YYSYMBOL_variable_declaration_with_instantiation = 57, /* variable_declaration_with_instantiation  */
  YYSYMBOL_optional_instantiation = 58,    /* optional_instantiation  */
  YYSYMBOL_literal = 59,                   /* literal  */
  YYSYMBOL_attribution_command = 60,       /* attribution_command  */
  YYSYMBOL_function_call = 61,             /* function_call  */
  YYSYMBOL_args = 62,                      /* args  */
  YYSYMBOL_args_list = 63,                 /* args_list  */
  YYSYMBOL_return_command = 64,            /* return_command  */
  YYSYMBOL_flow_control_command = 65,      /* flow_control_command  */
  YYSYMBOL_conditional_struct = 66,        /* conditional_struct  */
  YYSYMBOL_else_block = 67,                /* else_block  */
  YYSYMBOL_iterative_struct = 68,          /* iterative_struct  */
  YYSYMBOL_expression = 69,                /* expression  */
  YYSYMBOL_logical_or_expression = 70,     /* logical_or_expression  */
  YYSYMBOL_logical_and_expression = 71,    /* logical_and_expression  */
  YYSYMBOL_equality_expression = 72,       /* equality_expression  */
  YYSYMBOL_relational_expression = 73,     /* relational_expression  */
  YYSYMBOL_additive_expression = 74,       /* additive_expression  */
  YYSYMBOL_multiplicative_expression = 75, /* multiplicative_expression  */
  YYSYMBOL_unary_expression = 76,          /* unary_expression  */
  YYSYMBOL_primary_expression = 77         /* primary_expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  134

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   276


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,     2,     2,    36,    29,     2,
      26,    27,    34,    32,    23,    33,     2,    35,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    22,
      30,     2,    31,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    25,     2,    24,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    28,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    87,    87,    96,   102,   110,   113,   120,   121,   123,
     123,   149,   157,   158,   160,   161,   162,   164,   165,   173,
     179,   181,   186,   192,   197,   200,   205,   206,   207,   208,
     209,   210,   212,   220,   242,   246,   248,   255,   263,   280,
     311,   311,   313,   318,   324,   333,   333,   335,   362,   362,
     364,   384,   386,   387,   394,   395,   402,   403,   409,   416,
     417,   423,   429,   435,   442,   443,   451,   460,   461,   469,
     477,   484,   485,   489,   493,   499,   516,   523,   530,   531
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TK_TIPO", "TK_VAR",
  "TK_SENAO", "TK_DECIMAL", "TK_SE", "TK_INTEIRO", "TK_ATRIB",
  "TK_RETORNA", "TK_SETA", "TK_ENQUANTO", "TK_COM", "TK_OC_LE", "TK_OC_GE",
  "TK_OC_EQ", "TK_OC_NE", "TK_ID", "TK_LI_INTEIRO", "TK_LI_DECIMAL",
  "TK_ER", "';'", "','", "']'", "'['", "'('", "')'", "'|'", "'&'", "'<'",
  "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "$accept", "program",
  "global_escope_init", "global_escope_end", "list", "element",
  "function_definition", "$@1", "header", "var_type",
  "optional_parameter_list", "parameter_list", "parameter", "body",
  "command_block", "block_scope_init", "command_sequence",
  "simple_command", "variable_declaration",
  "variable_declaration_with_instantiation", "optional_instantiation",
  "literal", "attribution_command", "function_call", "args", "args_list",
  "return_command", "flow_control_command", "conditional_struct",
  "else_block", "iterative_struct", "expression", "logical_or_expression",
  "logical_and_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "primary_expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-53)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -53,    25,    19,   -53,    17,    37,   -53,    31,   -53,   -53,
     -53,    50,    -2,    42,    19,    49,    -2,   -53,   -53,    20,
     -53,   -53,   -53,   -53,   -53,     6,   -53,    57,    67,    68,
      55,   -53,    61,    54,    23,    56,    13,   -53,   -53,    59,
      22,   -53,   -53,   -53,   -53,   -53,   -53,   -53,    55,    -2,
     -53,    57,    72,    23,    58,   -53,   -53,    23,    23,    23,
      23,   -53,    76,    60,    62,    41,    -3,   -18,    27,   -53,
     -53,    23,    23,    23,   -53,   -53,   -53,   -53,    -2,    63,
      65,   -53,   -53,   -53,    -2,    23,    23,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    23,    23,    66,   -53,
      69,    64,   -53,    73,    49,   -53,   -53,    62,    41,    -3,
      -3,   -18,   -18,   -18,   -18,    27,    27,   -53,   -53,   -53,
      49,   -53,    23,    47,   -53,    89,   -53,   -53,   -53,   -53,
     -53,    49,   -53,   -53
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     0,     0,     4,     5,     7,     9,
       8,     0,     0,     0,     0,     0,     0,    12,    13,    14,
       2,     6,    23,    10,    20,     0,    32,     0,     0,     0,
      16,    17,     0,     0,     0,     0,     0,    22,    31,     0,
      24,    26,    28,    27,    29,    30,    45,    46,    15,     0,
      11,     0,     0,     0,    75,    76,    77,     0,     0,     0,
       0,    79,     0,    51,    52,    54,    56,    59,    64,    67,
      71,     0,     0,    41,    21,    25,    19,    18,     0,     0,
       0,    73,    74,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
       0,    40,    42,    35,     0,    78,    44,    53,    55,    57,
      58,    62,    63,    60,    61,    65,    66,    68,    69,    70,
       0,    39,     0,     0,    33,    49,    50,    43,    36,    37,
      34,     0,    47,    48
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -53,   -53,   -53,   -53,    81,   -53,   -53,   -53,   -53,   -13,
     -53,    70,    48,   -53,   -15,   -53,    71,   -53,   -53,   -53,
     -53,   -53,   -53,   -23,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,   -52,   -53,    15,    12,   -19,   -39,   -21,   -51,   -53
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    13,     6,     7,     8,    15,     9,    19,
      29,    30,    31,    23,    38,    25,    39,    40,    10,    41,
     124,   130,    42,    61,   100,   101,    44,    45,    46,   132,
      47,    62,    63,    64,    65,    66,    67,    68,    69,    70
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      24,    79,    43,    26,    17,    80,    18,    81,    82,    83,
      32,    89,    90,    33,    93,    94,    34,    43,    35,    98,
      99,   102,    72,     4,    36,     3,    32,    91,    92,    33,
      37,    22,    34,    27,    35,    11,    76,     5,    28,    73,
      36,    54,    55,    56,   117,   118,   119,    22,    12,    57,
     111,   112,   113,   114,    14,    58,    59,    87,    88,    16,
      60,    95,    96,    97,    20,   103,   128,   129,   109,   110,
     127,   106,   115,   116,    22,    28,    49,    50,    51,    52,
      53,    78,    71,    74,    73,    84,   123,   122,    85,   125,
     104,    86,   105,   120,   131,    21,   121,    48,   108,    77,
     107,     0,     0,     0,     0,   126,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,   133
};

static const yytype_int16 yycheck[] =
{
      15,    53,    25,    16,     6,    57,     8,    58,    59,    60,
       4,    14,    15,     7,    32,    33,    10,    40,    12,    71,
      72,    73,     9,     4,    18,     0,     4,    30,    31,     7,
      24,    25,    10,    13,    12,    18,    49,    18,    18,    26,
      18,    18,    19,    20,    95,    96,    97,    25,    11,    26,
      89,    90,    91,    92,    23,    32,    33,    16,    17,     9,
      37,    34,    35,    36,    22,    78,    19,    20,    87,    88,
     122,    84,    93,    94,    25,    18,     9,     9,    23,    18,
      26,     9,    26,    24,    26,     9,    13,    23,    28,   104,
      27,    29,    27,    27,     5,    14,    27,    27,    86,    51,
      85,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   131
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    39,    40,     0,     4,    18,    42,    43,    44,    46,
      56,    18,    11,    41,    23,    45,     9,     6,     8,    47,
      22,    42,    25,    51,    52,    53,    47,    13,    18,    48,
      49,    50,     4,     7,    10,    12,    18,    24,    52,    54,
      55,    57,    60,    61,    64,    65,    66,    68,    49,     9,
       9,    23,    18,    26,    18,    19,    20,    26,    32,    33,
      37,    61,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    26,     9,    26,    24,    54,    47,    50,     9,    69,
      69,    76,    76,    76,     9,    28,    29,    16,    17,    14,
      15,    30,    31,    32,    33,    34,    35,    36,    69,    69,
      62,    63,    69,    47,    27,    27,    47,    71,    72,    73,
      73,    74,    74,    74,    74,    75,    75,    76,    76,    76,
      27,    27,    23,    13,    58,    52,    52,    69,    19,    20,
      59,     5,    67,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    41,    42,    42,    43,    43,    45,
      44,    46,    47,    47,    48,    48,    48,    49,    49,    50,
      51,    52,    52,    53,    54,    54,    55,    55,    55,    55,
      55,    55,    56,    57,    58,    58,    59,    59,    60,    61,
      62,    62,    63,    63,    64,    65,    65,    66,    67,    67,
      68,    69,    70,    70,    71,    71,    72,    72,    72,    73,
      73,    73,    73,    73,    74,    74,    74,    75,    75,    75,
      75,    76,    76,    76,    76,    77,    77,    77,    77,    77
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     0,     0,     1,     3,     1,     1,     0,
       3,     5,     1,     1,     0,     2,     1,     1,     3,     3,
       1,     3,     2,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     4,     5,     2,     0,     1,     1,     3,     4,
       1,     0,     1,     3,     4,     1,     1,     6,     2,     0,
       5,     1,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     2,     2,     1,     1,     1,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: global_escope_init list global_escope_end ';'  */
#line 87 "parser/parser.y"
                                                       {
  arvore = (yyvsp[-2].node);
  if (arvore && arvore->code) {
      iloc_print_code(arvore->code);
      iloc_code_free(arvore->code);
      arvore->code = NULL;
  }
}
#line 1556 "parser/parser.tab.c"
    break;

  case 3: /* global_escope_init: %empty  */
#line 96 "parser/parser.y"
                           { 
  (yyval.node) = NULL;
  if (!scope_stack_pointer) scope_stack_pointer = scope_stack_create();
  scope_push(scope_stack_pointer, SCOPE_GLOBAL);
 }
#line 1566 "parser/parser.tab.c"
    break;

  case 4: /* global_escope_end: %empty  */
#line 102 "parser/parser.y"
                          {
  (yyval.node) = NULL;
  scope_log_global_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
  scope_stack_destroy(scope_stack_pointer);
  scope_stack_pointer = NULL;
 }
#line 1578 "parser/parser.tab.c"
    break;

  case 5: /* list: element  */
#line 110 "parser/parser.y"
              {
  (yyval.node) = (yyvsp[0].node);
}
#line 1586 "parser/parser.tab.c"
    break;

  case 6: /* list: element ',' list  */
#line 113 "parser/parser.y"
                   {
  (yyval.node) = asd_select_head_and_attach_tail((yyvsp[-2].node), (yyvsp[0].node));
  if ((yyvsp[-2].node) && (yyvsp[0].node)) {
      (yyvsp[-2].node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  }
}
#line 1597 "parser/parser.tab.c"
    break;

  case 7: /* element: function_definition  */
#line 120 "parser/parser.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1603 "parser/parser.tab.c"
    break;

  case 8: /* element: variable_declaration  */
#line 121 "parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1609 "parser/parser.tab.c"
    break;

  case 9: /* $@1: %empty  */
#line 123 "parser/parser.y"
                            {
  scope_push(scope_stack_pointer, SCOPE_FUNCTION);
  current_function_return_type = (yyvsp[0].symbol)->data_type;

  for(arg_type_node_t *arg = (yyvsp[0].symbol)->args; arg; arg = arg->next) {
    lexical_value_t param_lex;
    param_lex.line_number = (yyvsp[0].symbol)->lexical.line_number;
    param_lex.token_type = "param";
    param_lex.value = arg->name; 
    if(!scope_insert_current(scope_stack_pointer, arg->name, SYMBOL_VARIABLE, arg->type, &param_lex)) {
      semantic_error(11, param_lex.line_number, arg->name);
    }
  }
}
#line 1628 "parser/parser.tab.c"
    break;

  case 10: /* function_definition: header $@1 body  */
#line 136 "parser/parser.y"
       {
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "function %s", (yyvsp[-2].symbol)->key);
  (yyval.node) = asd_new(buffer);
  (yyval.node)->type = (yyvsp[-2].symbol)->data_type;
  asd_add_child((yyval.node), (yyvsp[0].node));
  if ((yyvsp[0].node)) (yyval.node)->code = (yyvsp[0].node)->code;
  
  scope_log_function_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
  current_function_return_type = TYPE_UNDEFINED;
}
#line 1645 "parser/parser.tab.c"
    break;

  case 11: /* header: TK_ID TK_SETA var_type optional_parameter_list TK_ATRIB  */
#line 149 "parser/parser.y"
                                                                {
  symbol_entry_t *entry = scope_insert_current(scope_stack_pointer, (yyvsp[-4].lexical_value).value, SYMBOL_FUNCTION, (yyvsp[-2].data_type), &(yyvsp[-4].lexical_value));
  if(!entry) semantic_error(11, (yyvsp[-4].lexical_value).line_number, (yyvsp[-4].lexical_value).value);
  entry->args = (yyvsp[-1].arg_list);
  (yyval.symbol) = entry;
  free((yyvsp[-4].lexical_value).value);
}
#line 1657 "parser/parser.tab.c"
    break;

  case 12: /* var_type: TK_DECIMAL  */
#line 157 "parser/parser.y"
                     { (yyval.data_type) = TYPE_FLOAT; }
#line 1663 "parser/parser.tab.c"
    break;

  case 13: /* var_type: TK_INTEIRO  */
#line 158 "parser/parser.y"
             { (yyval.data_type) = TYPE_INT; }
#line 1669 "parser/parser.tab.c"
    break;

  case 14: /* optional_parameter_list: %empty  */
#line 160 "parser/parser.y"
                                { (yyval.arg_list) = NULL; }
#line 1675 "parser/parser.tab.c"
    break;

  case 15: /* optional_parameter_list: TK_COM parameter_list  */
#line 161 "parser/parser.y"
                        { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 1681 "parser/parser.tab.c"
    break;

  case 16: /* optional_parameter_list: parameter_list  */
#line 162 "parser/parser.y"
                 { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 1687 "parser/parser.tab.c"
    break;

  case 17: /* parameter_list: parameter  */
#line 164 "parser/parser.y"
                          { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 1693 "parser/parser.tab.c"
    break;

  case 18: /* parameter_list: parameter_list ',' parameter  */
#line 165 "parser/parser.y"
                               {
  arg_type_node_t *head = (yyvsp[-2].arg_list);
  arg_type_node_t *cur = head;
  while (cur && cur->next) cur = cur->next;
  if (cur) cur->next = (yyvsp[0].arg_list);
  (yyval.arg_list) = head;
}
#line 1705 "parser/parser.tab.c"
    break;

  case 19: /* parameter: TK_ID TK_ATRIB var_type  */
#line 173 "parser/parser.y"
                                   {
  (yyval.arg_list) = NULL;
  (yyval.arg_list) = args_append((yyval.arg_list), (yyvsp[0].data_type), (yyvsp[-2].lexical_value).value);
  free((yyvsp[-2].lexical_value).value);
}
#line 1715 "parser/parser.tab.c"
    break;

  case 20: /* body: command_block  */
#line 179 "parser/parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1721 "parser/parser.tab.c"
    break;

  case 21: /* command_block: block_scope_init command_sequence ']'  */
#line 181 "parser/parser.y"
                                                     {
  (yyval.node) = (yyvsp[-1].node);
  scope_log_block_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
}
#line 1731 "parser/parser.tab.c"
    break;

  case 22: /* command_block: block_scope_init ']'  */
#line 186 "parser/parser.y"
                       {
  (yyval.node) = NULL;
  scope_log_block_end(scope_stack_pointer);
  scope_pop(scope_stack_pointer);
}
#line 1741 "parser/parser.tab.c"
    break;

  case 23: /* block_scope_init: '['  */
#line 192 "parser/parser.y"
                      {
  (yyval.node) = NULL;
  scope_push(scope_stack_pointer, SCOPE_BLOCK);
}
#line 1750 "parser/parser.tab.c"
    break;

  case 24: /* command_sequence: simple_command  */
#line 197 "parser/parser.y"
                                 {
  (yyval.node) = (yyvsp[0].node);
}
#line 1758 "parser/parser.tab.c"
    break;

  case 25: /* command_sequence: simple_command command_sequence  */
#line 200 "parser/parser.y"
                                  {
  (yyval.node) = asd_select_head_and_attach_tail((yyvsp[-1].node), (yyvsp[0].node));
  if ((yyvsp[-1].node) && (yyvsp[0].node)) (yyvsp[-1].node)->code = iloc_append((yyvsp[-1].node)->code, (yyvsp[0].node)->code);
}
#line 1767 "parser/parser.tab.c"
    break;

  case 26: /* simple_command: variable_declaration_with_instantiation  */
#line 205 "parser/parser.y"
                                                        { (yyval.node) = (yyvsp[0].node); }
#line 1773 "parser/parser.tab.c"
    break;

  case 27: /* simple_command: function_call  */
#line 206 "parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1779 "parser/parser.tab.c"
    break;

  case 28: /* simple_command: attribution_command  */
#line 207 "parser/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1785 "parser/parser.tab.c"
    break;

  case 29: /* simple_command: return_command  */
#line 208 "parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1791 "parser/parser.tab.c"
    break;

  case 30: /* simple_command: flow_control_command  */
#line 209 "parser/parser.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1797 "parser/parser.tab.c"
    break;

  case 31: /* simple_command: command_block  */
#line 210 "parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1803 "parser/parser.tab.c"
    break;

  case 32: /* variable_declaration: TK_VAR TK_ID TK_ATRIB var_type  */
#line 212 "parser/parser.y"
                                                     {
  if(!scope_insert_current(scope_stack_pointer, (yyvsp[-2].lexical_value).value, SYMBOL_VARIABLE, (yyvsp[0].data_type), &(yyvsp[-2].lexical_value))) {
    semantic_error(11, (yyvsp[-2].lexical_value).line_number, (yyvsp[-2].lexical_value).value);
  }
  (yyval.node) = NULL;  
  free((yyvsp[-2].lexical_value).value);
}
#line 1815 "parser/parser.tab.c"
    break;

  case 33: /* variable_declaration_with_instantiation: TK_VAR TK_ID TK_ATRIB var_type optional_instantiation  */
#line 220 "parser/parser.y"
                                                                                               {
  symbol_entry_t *entry = scope_insert_current(scope_stack_pointer, (yyvsp[-3].lexical_value).value, SYMBOL_VARIABLE, (yyvsp[-1].data_type), &(yyvsp[-3].lexical_value));
  if (!entry) semantic_error(11, (yyvsp[-3].lexical_value).line_number, (yyvsp[-3].lexical_value).value);

  asd_tree_t *inst_node = (yyvsp[0].node);
  if (inst_node != NULL) {
    if (inst_node->type != (yyvsp[-1].data_type)) semantic_error(30, (yyvsp[-3].lexical_value).line_number, "Instantiation type mismatch");
    int base_reg = entry->is_global ? -2 : -1;
    iloc_code_t *store = iloc_create_inst(ILOC_STOREAI, ILOC_OP_REG, get_reg_id(inst_node->temp), ILOC_OP_REG, base_reg, ILOC_OP_INT, entry->offset);
    inst_node->code = iloc_append(inst_node->code, store);
  }

  (yyval.node) = inst_node;
  if ((yyval.node) != NULL) {
    asd_tree_t *idnode = asd_new_node_from_value(&(yyvsp[-3].lexical_value));
    if (idnode) idnode->type = (yyvsp[-1].data_type);
    asd_add_child((yyval.node), idnode);
  } else {
    free((yyvsp[-3].lexical_value).value);
  }
}
#line 1841 "parser/parser.tab.c"
    break;

  case 34: /* optional_instantiation: TK_COM literal  */
#line 242 "parser/parser.y"
                                       { 
  (yyval.node) = asd_new_unary("com", (yyvsp[0].node));
  (yyval.node)->type = (yyvsp[0].node)->type; (yyval.node)->code = (yyvsp[0].node)->code; (yyval.node)->temp = (yyvsp[0].node)->temp;
}
#line 1850 "parser/parser.tab.c"
    break;

  case 35: /* optional_instantiation: %empty  */
#line 246 "parser/parser.y"
         { (yyval.node) = NULL; }
#line 1856 "parser/parser.tab.c"
    break;

  case 36: /* literal: TK_LI_INTEIRO  */
#line 248 "parser/parser.y"
                       {
  int val = atoi((yyvsp[0].lexical_value).value);
  (yyval.node) = asd_new_node_from_value(&(yyvsp[0].lexical_value));
  (yyval.node)->type = TYPE_INT;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, ILOC_OP_REG, get_reg_id((yyval.node)->temp), 0,0);
}
#line 1868 "parser/parser.tab.c"
    break;

  case 37: /* literal: TK_LI_DECIMAL  */
#line 255 "parser/parser.y"
                {
  int val = (int)atof((yyvsp[0].lexical_value).value);
  (yyval.node) = asd_new_node_from_value(&(yyvsp[0].lexical_value));
  (yyval.node)->type = TYPE_FLOAT;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, ILOC_OP_REG, get_reg_id((yyval.node)->temp), 0,0);
}
#line 1880 "parser/parser.tab.c"
    break;

  case 38: /* attribution_command: TK_ID TK_ATRIB expression  */
#line 263 "parser/parser.y"
                                               {
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, (yyvsp[-2].lexical_value).value);
  if (!entry) semantic_error(10, (yyvsp[-2].lexical_value).line_number, (yyvsp[-2].lexical_value).value);
  if (entry->nature == SYMBOL_FUNCTION) semantic_error(21, (yyvsp[-2].lexical_value).line_number, (yyvsp[-2].lexical_value).value);
  if (entry->nature != SYMBOL_VARIABLE) semantic_error(20, (yyvsp[-2].lexical_value).line_number, (yyvsp[-2].lexical_value).value);
  
  if (entry->data_type != (yyvsp[0].node)->type) semantic_error(30, (yyvsp[-2].lexical_value).line_number, "Attribution type mismatch");

  asd_tree_t *idnode = asd_new_node_from_value(&(yyvsp[-2].lexical_value));
  (yyval.node) = asd_new_binary(":=", idnode, (yyvsp[0].node));
  (yyval.node)->type = entry->data_type;

  int base_reg = entry->is_global ? -2 : -1;
  iloc_code_t *store = iloc_create_inst(ILOC_STOREAI, ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, base_reg, ILOC_OP_INT, entry->offset);
  (yyval.node)->code = iloc_append((yyvsp[0].node)->code, store);
}
#line 1901 "parser/parser.tab.c"
    break;

  case 39: /* function_call: TK_ID '(' args ')'  */
#line 280 "parser/parser.y"
                                  {
    symbol_entry_t *entry = scope_lookup(scope_stack_pointer, (yyvsp[-3].lexical_value).value);
    if (!entry) semantic_error(10, (yyvsp[-3].lexical_value).line_number, (yyvsp[-3].lexical_value).value);
    if (entry->nature != SYMBOL_FUNCTION) semantic_error(20, (yyvsp[-3].lexical_value).line_number, (yyvsp[-3].lexical_value).value);
    
    int count_given = 0;
    asd_tree_t *args_node = (yyvsp[-1].node);
    if (args_node) count_given = args_node->number_of_children;

    int count_expected = 0;
    arg_type_node_t *p = entry->args;
    while(p) { count_expected++; p = p->next; }

    if (count_given < count_expected) semantic_error(40, (yyvsp[-3].lexical_value).line_number, "Missing arguments");
    if (count_given > count_expected) semantic_error(41, (yyvsp[-3].lexical_value).line_number, "Excess arguments");

    p = entry->args;
    if (args_node) {
        for (int i = 0; i < args_node->number_of_children; i++) {
            asd_tree_t *arg_expr = args_node->children[i];
            if (arg_expr->type != p->type) semantic_error(42, (yyvsp[-3].lexical_value).line_number, "Argument type mismatch");
            p = p->next;
        }
    }

    (yyval.node) = asd_new_function_call_node(&(yyvsp[-3].lexical_value), (yyvsp[-1].node));
    (yyval.node)->type = entry->data_type;
    if ((yyvsp[-1].node)) (yyval.node)->code = (yyvsp[-1].node)->code;
    (yyval.node)->temp = iloc_new_reg(); 
}
#line 1936 "parser/parser.tab.c"
    break;

  case 40: /* args: args_list  */
#line 311 "parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1942 "parser/parser.tab.c"
    break;

  case 41: /* args: %empty  */
#line 311 "parser/parser.y"
                                      { (yyval.node) = NULL; }
#line 1948 "parser/parser.tab.c"
    break;

  case 42: /* args_list: expression  */
#line 313 "parser/parser.y"
                      {
    (yyval.node) = asd_new("args");
    asd_add_child((yyval.node), (yyvsp[0].node));
    (yyval.node)->code = (yyvsp[0].node)->code;
}
#line 1958 "parser/parser.tab.c"
    break;

  case 43: /* args_list: args_list ',' expression  */
#line 318 "parser/parser.y"
                           {
    (yyval.node) = (yyvsp[-2].node);
    asd_add_child((yyval.node), (yyvsp[0].node));
    (yyval.node)->code = iloc_append((yyval.node)->code, (yyvsp[0].node)->code);
}
#line 1968 "parser/parser.tab.c"
    break;

  case 44: /* return_command: TK_RETORNA expression TK_ATRIB var_type  */
#line 324 "parser/parser.y"
                                                        {
  if ((yyvsp[-2].node)->type != (yyvsp[0].data_type)) semantic_error(30, yylineno, "Return command type mismatch");
  if (current_function_return_type != TYPE_UNDEFINED && (yyvsp[0].data_type) != current_function_return_type) {
      semantic_error(30, yylineno, "Function return type mismatch");
  }
  (yyval.node) = asd_new_unary("retorna", (yyvsp[-2].node));
  (yyval.node)->code = (yyvsp[-2].node)->code;
}
#line 1981 "parser/parser.tab.c"
    break;

  case 45: /* flow_control_command: conditional_struct  */
#line 333 "parser/parser.y"
                                         { (yyval.node) = (yyvsp[0].node); }
#line 1987 "parser/parser.tab.c"
    break;

  case 46: /* flow_control_command: iterative_struct  */
#line 333 "parser/parser.y"
                                                                         { (yyval.node) = (yyvsp[0].node); }
#line 1993 "parser/parser.tab.c"
    break;

  case 47: /* conditional_struct: TK_SE '(' expression ')' command_block else_block  */
#line 335 "parser/parser.y"
                                                                      {
  (yyval.node) = asd_new_trinary("se", (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));
  char *L_true = iloc_new_label(), *L_false = iloc_new_label();
  char *L_saida = ((yyvsp[0].node) != NULL) ? iloc_new_label() : L_false;

  iloc_code_t *cbr = iloc_create_inst(ILOC_CBR, ILOC_OP_REG, get_reg_id((yyvsp[-3].node)->temp), ILOC_OP_LABEL, atoi(L_true+1), ILOC_OP_LABEL, atoi(L_false+1));
  iloc_code_t *l_true = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_true+1), 0,0,0,0);
  iloc_code_t *l_false = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_false+1), 0,0,0,0);
  
  (yyval.node)->code = iloc_append((yyvsp[-3].node)->code, cbr);
  (yyval.node)->code = iloc_append((yyval.node)->code, l_true);
  if ((yyvsp[-1].node)) (yyval.node)->code = iloc_append((yyval.node)->code, (yyvsp[-1].node)->code);

  if ((yyvsp[0].node) != NULL) {
      iloc_code_t *jmp = iloc_create_inst(ILOC_JUMPI, 0,0,0,0, ILOC_OP_LABEL, atoi(L_saida+1));
      (yyval.node)->code = iloc_append((yyval.node)->code, jmp);
      (yyval.node)->code = iloc_append((yyval.node)->code, l_false);
      (yyval.node)->code = iloc_append((yyval.node)->code, (yyvsp[0].node)->code);
      iloc_code_t *l_out = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_saida+1), 0,0,0,0);
      (yyval.node)->code = iloc_append((yyval.node)->code, l_out);
      free(L_saida);
  } else {
      (yyval.node)->code = iloc_append((yyval.node)->code, l_false);
  }
  free(L_true); free(L_false);
}
#line 2024 "parser/parser.tab.c"
    break;

  case 48: /* else_block: TK_SENAO command_block  */
#line 362 "parser/parser.y"
                                   { (yyval.node) = (yyvsp[0].node); }
#line 2030 "parser/parser.tab.c"
    break;

  case 49: /* else_block: %empty  */
#line 362 "parser/parser.y"
                                                         { (yyval.node) = NULL; }
#line 2036 "parser/parser.tab.c"
    break;

  case 50: /* iterative_struct: TK_ENQUANTO '(' expression ')' command_block  */
#line 364 "parser/parser.y"
                                                               {
  (yyval.node) = asd_new_binary("enquanto", (yyvsp[-2].node), (yyvsp[0].node));
  char *L_ini = iloc_new_label(), *L_true = iloc_new_label(), *L_out = iloc_new_label();

  iloc_code_t *l_ini = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_ini+1), 0,0,0,0);
  iloc_code_t *l_true = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_true+1), 0,0,0,0);
  iloc_code_t *l_out = iloc_create_inst(ILOC_NOP, ILOC_OP_LABEL, atoi(L_out+1), 0,0,0,0);
  iloc_code_t *cbr = iloc_create_inst(ILOC_CBR, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_LABEL, atoi(L_true+1), ILOC_OP_LABEL, atoi(L_out+1));
  iloc_code_t *jmp = iloc_create_inst(ILOC_JUMPI, 0,0,0,0, ILOC_OP_LABEL, atoi(L_ini+1));

  (yyval.node)->code = l_ini;
  (yyval.node)->code = iloc_append((yyval.node)->code, (yyvsp[-2].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, cbr);
  (yyval.node)->code = iloc_append((yyval.node)->code, l_true);
  if ((yyvsp[0].node)) (yyval.node)->code = iloc_append((yyval.node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, jmp);
  (yyval.node)->code = iloc_append((yyval.node)->code, l_out);
  free(L_ini); free(L_true); free(L_out);
}
#line 2060 "parser/parser.tab.c"
    break;

  case 51: /* expression: logical_or_expression  */
#line 384 "parser/parser.y"
                                  { (yyval.node) = (yyvsp[0].node); }
#line 2066 "parser/parser.tab.c"
    break;

  case 52: /* logical_or_expression: logical_and_expression  */
#line 386 "parser/parser.y"
                                              { (yyval.node) = (yyvsp[0].node); }
#line 2072 "parser/parser.tab.c"
    break;

  case 53: /* logical_or_expression: logical_or_expression '|' logical_and_expression  */
#line 387 "parser/parser.y"
                                                   {
  (yyval.node) = asd_new_binary("|", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_OR, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2083 "parser/parser.tab.c"
    break;

  case 54: /* logical_and_expression: equality_expression  */
#line 394 "parser/parser.y"
                                            { (yyval.node) = (yyvsp[0].node); }
#line 2089 "parser/parser.tab.c"
    break;

  case 55: /* logical_and_expression: logical_and_expression '&' equality_expression  */
#line 395 "parser/parser.y"
                                                 {
  (yyval.node) = asd_new_binary("&", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_AND, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2100 "parser/parser.tab.c"
    break;

  case 56: /* equality_expression: relational_expression  */
#line 402 "parser/parser.y"
                                           { (yyval.node) = (yyvsp[0].node); }
#line 2106 "parser/parser.tab.c"
    break;

  case 57: /* equality_expression: equality_expression TK_OC_EQ relational_expression  */
#line 403 "parser/parser.y"
                                                     {
  (yyval.node) = asd_new_binary("==", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_CMP_EQ, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2117 "parser/parser.tab.c"
    break;

  case 58: /* equality_expression: equality_expression TK_OC_NE relational_expression  */
#line 409 "parser/parser.y"
                                                     {
  (yyval.node) = asd_new_binary("!=", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_CMP_NE, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2128 "parser/parser.tab.c"
    break;

  case 59: /* relational_expression: additive_expression  */
#line 416 "parser/parser.y"
                                           { (yyval.node) = (yyvsp[0].node); }
#line 2134 "parser/parser.tab.c"
    break;

  case 60: /* relational_expression: relational_expression '<' additive_expression  */
#line 417 "parser/parser.y"
                                                {
  (yyval.node) = asd_new_binary("<", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_CMP_LT, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2145 "parser/parser.tab.c"
    break;

  case 61: /* relational_expression: relational_expression '>' additive_expression  */
#line 423 "parser/parser.y"
                                                {
  (yyval.node) = asd_new_binary(">", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_CMP_GT, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2156 "parser/parser.tab.c"
    break;

  case 62: /* relational_expression: relational_expression TK_OC_LE additive_expression  */
#line 429 "parser/parser.y"
                                                     {
  (yyval.node) = asd_new_binary("<=", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_CMP_LE, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2167 "parser/parser.tab.c"
    break;

  case 63: /* relational_expression: relational_expression TK_OC_GE additive_expression  */
#line 435 "parser/parser.y"
                                                     {
  (yyval.node) = asd_new_binary(">=", (yyvsp[-2].node), (yyvsp[0].node));
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_CMP_GE, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2178 "parser/parser.tab.c"
    break;

  case 64: /* additive_expression: multiplicative_expression  */
#line 442 "parser/parser.y"
                                               { (yyval.node) = (yyvsp[0].node); }
#line 2184 "parser/parser.tab.c"
    break;

  case 65: /* additive_expression: additive_expression '+' multiplicative_expression  */
#line 443 "parser/parser.y"
                                                    {
  (yyval.node) = asd_new_binary("+", (yyvsp[-2].node), (yyvsp[0].node));
  if ((yyvsp[-2].node)->type != (yyvsp[0].node)->type) semantic_error(30, yylineno, "Type mismatch add");
  (yyval.node)->type = (yyvsp[-2].node)->type;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_ADD, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2197 "parser/parser.tab.c"
    break;

  case 66: /* additive_expression: additive_expression '-' multiplicative_expression  */
#line 451 "parser/parser.y"
                                                    {
  (yyval.node) = asd_new_binary("-", (yyvsp[-2].node), (yyvsp[0].node));
  if ((yyvsp[-2].node)->type != (yyvsp[0].node)->type) semantic_error(30, yylineno, "Type mismatch sub");
  (yyval.node)->type = (yyvsp[-2].node)->type;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_SUB, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2210 "parser/parser.tab.c"
    break;

  case 67: /* multiplicative_expression: unary_expression  */
#line 460 "parser/parser.y"
                                            { (yyval.node) = (yyvsp[0].node); }
#line 2216 "parser/parser.tab.c"
    break;

  case 68: /* multiplicative_expression: multiplicative_expression '*' unary_expression  */
#line 461 "parser/parser.y"
                                                 {
  (yyval.node) = asd_new_binary("*", (yyvsp[-2].node), (yyvsp[0].node));
  if ((yyvsp[-2].node)->type != (yyvsp[0].node)->type) semantic_error(30, yylineno, "Type mismatch mult");
  (yyval.node)->type = (yyvsp[-2].node)->type;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_MULT, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2229 "parser/parser.tab.c"
    break;

  case 69: /* multiplicative_expression: multiplicative_expression '/' unary_expression  */
#line 469 "parser/parser.y"
                                                 {
  (yyval.node) = asd_new_binary("/", (yyvsp[-2].node), (yyvsp[0].node));
  if ((yyvsp[-2].node)->type != (yyvsp[0].node)->type) semantic_error(30, yylineno, "Type mismatch div");
  (yyval.node)->type = (yyvsp[-2].node)->type;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
  (yyval.node)->code = iloc_append((yyval.node)->code, iloc_create_inst(ILOC_DIV, ILOC_OP_REG, get_reg_id((yyvsp[-2].node)->temp), ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2242 "parser/parser.tab.c"
    break;

  case 70: /* multiplicative_expression: multiplicative_expression '%' unary_expression  */
#line 477 "parser/parser.y"
                                                 {
  (yyval.node) = asd_new_binary("%", (yyvsp[-2].node), (yyvsp[0].node));
  if ((yyvsp[-2].node)->type != TYPE_INT || (yyvsp[0].node)->type != TYPE_INT) semantic_error(30, yylineno, "Modulo requires int");
  (yyval.node)->type = TYPE_INT; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[-2].node)->code, (yyvsp[0].node)->code);
}
#line 2253 "parser/parser.tab.c"
    break;

  case 71: /* unary_expression: primary_expression  */
#line 484 "parser/parser.y"
                                     { (yyval.node) = (yyvsp[0].node); }
#line 2259 "parser/parser.tab.c"
    break;

  case 72: /* unary_expression: '!' unary_expression  */
#line 485 "parser/parser.y"
                       { 
  (yyval.node) = asd_new_unary("!", (yyvsp[0].node)); 
  (yyval.node)->type = TYPE_INT; (yyval.node)->code = (yyvsp[0].node)->code;
}
#line 2268 "parser/parser.tab.c"
    break;

  case 73: /* unary_expression: '+' unary_expression  */
#line 489 "parser/parser.y"
                       { 
  (yyval.node) = asd_new_unary("+", (yyvsp[0].node)); 
  (yyval.node)->type = (yyvsp[0].node)->type; (yyval.node)->code = (yyvsp[0].node)->code; (yyval.node)->temp = (yyvsp[0].node)->temp;
}
#line 2277 "parser/parser.tab.c"
    break;

  case 74: /* unary_expression: '-' unary_expression  */
#line 493 "parser/parser.y"
                       {
  (yyval.node) = asd_new_unary("-", (yyvsp[0].node));
  (yyval.node)->type = (yyvsp[0].node)->type; (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_append((yyvsp[0].node)->code, iloc_create_inst(ILOC_RSUBI, ILOC_OP_REG, get_reg_id((yyvsp[0].node)->temp), ILOC_OP_INT, 0, ILOC_OP_REG, get_reg_id((yyval.node)->temp)));
}
#line 2287 "parser/parser.tab.c"
    break;

  case 75: /* primary_expression: TK_ID  */
#line 499 "parser/parser.y"
                          {
  symbol_entry_t *entry = scope_lookup(scope_stack_pointer, (yyvsp[0].lexical_value).value);
  if (!entry) semantic_error(10, (yyvsp[0].lexical_value).line_number, (yyvsp[0].lexical_value).value);
  if (entry->nature == SYMBOL_FUNCTION) semantic_error(21, (yyvsp[0].lexical_value).line_number, (yyvsp[0].lexical_value).value);
  if (entry->nature != SYMBOL_VARIABLE) semantic_error(20, (yyvsp[0].lexical_value).line_number, (yyvsp[0].lexical_value).value);
  
  data_type_t type = entry->data_type;
  int offset = entry->offset;
  int is_global = entry->is_global;

  (yyval.node) = asd_new_node_from_value(&(yyvsp[0].lexical_value));
  (yyval.node)->type = type;
  (yyval.node)->temp = iloc_new_reg();
  int base_reg = is_global ? -2 : -1;
  
  (yyval.node)->code = iloc_create_inst(ILOC_LOADAI, ILOC_OP_REG, base_reg, ILOC_OP_INT, offset, ILOC_OP_REG, get_reg_id((yyval.node)->temp));
}
#line 2309 "parser/parser.tab.c"
    break;

  case 76: /* primary_expression: TK_LI_INTEIRO  */
#line 516 "parser/parser.y"
                {
  int val = atoi((yyvsp[0].lexical_value).value);
  (yyval.node) = asd_new_node_from_value(&(yyvsp[0].lexical_value));
  (yyval.node)->type = TYPE_INT;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, ILOC_OP_REG, get_reg_id((yyval.node)->temp), 0,0);
}
#line 2321 "parser/parser.tab.c"
    break;

  case 77: /* primary_expression: TK_LI_DECIMAL  */
#line 523 "parser/parser.y"
                {
  int val = (int)atof((yyvsp[0].lexical_value).value);
  (yyval.node) = asd_new_node_from_value(&(yyvsp[0].lexical_value));
  (yyval.node)->type = TYPE_FLOAT;
  (yyval.node)->temp = iloc_new_reg();
  (yyval.node)->code = iloc_create_inst(ILOC_LOADI, ILOC_OP_INT, val, ILOC_OP_REG, get_reg_id((yyval.node)->temp), 0,0);
}
#line 2333 "parser/parser.tab.c"
    break;

  case 78: /* primary_expression: '(' expression ')'  */
#line 530 "parser/parser.y"
                     { (yyval.node) = (yyvsp[-1].node); }
#line 2339 "parser/parser.tab.c"
    break;

  case 79: /* primary_expression: function_call  */
#line 531 "parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2345 "parser/parser.tab.c"
    break;


#line 2349 "parser/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 533 "parser/parser.y"


void yyerror (char const *mensagem) {
 extern int yylineno;
 printf("ERROR FOUND at line %d: [%s]\n", yylineno, mensagem);
}

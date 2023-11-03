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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  std::cout << "yyerror err msg: " << msg << std::endl;
  std::cout << sql_string << std::endl;
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

static inline void modify_2_negative(Value *value) {
  AttrType type = value->attr_type();
  if (type == AttrType::INTS) {
    value->set_int(value->get_int() * -1);
  }else if(type == AttrType::FLOATS) {
    value->set_float(value->get_float() * -1);
  }
}

void init_relation_sql_node(char *table_name, char *alias, RelationSqlNode &node) {
  node.table = table_name;
  node.alias.reset(alias);
}


#line 131 "yacc_sql.cpp"

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

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_CALC = 9,                       /* CALC  */
  YYSYMBOL_SELECT = 10,                    /* SELECT  */
  YYSYMBOL_SHOW = 11,                      /* SHOW  */
  YYSYMBOL_SYNC = 12,                      /* SYNC  */
  YYSYMBOL_INSERT = 13,                    /* INSERT  */
  YYSYMBOL_DELETE = 14,                    /* DELETE  */
  YYSYMBOL_UPDATE = 15,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 16,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 17,                    /* RBRACE  */
  YYSYMBOL_COMMA = 18,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 19,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 20,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 21,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 22,                     /* INT_T  */
  YYSYMBOL_STRING_T = 23,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 24,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 25,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 26,                    /* TEXT_T  */
  YYSYMBOL_null = 27,                      /* null  */
  YYSYMBOL_IS = 28,                        /* IS  */
  YYSYMBOL_NOT = 29,                       /* NOT  */
  YYSYMBOL_HELP = 30,                      /* HELP  */
  YYSYMBOL_EXIT = 31,                      /* EXIT  */
  YYSYMBOL_DOT = 32,                       /* DOT  */
  YYSYMBOL_INTO = 33,                      /* INTO  */
  YYSYMBOL_VALUES = 34,                    /* VALUES  */
  YYSYMBOL_FROM = 35,                      /* FROM  */
  YYSYMBOL_WHERE = 36,                     /* WHERE  */
  YYSYMBOL_HAVING = 37,                    /* HAVING  */
  YYSYMBOL_AND = 38,                       /* AND  */
  YYSYMBOL_OR = 39,                        /* OR  */
  YYSYMBOL_SET = 40,                       /* SET  */
  YYSYMBOL_INNER = 41,                     /* INNER  */
  YYSYMBOL_JOIN = 42,                      /* JOIN  */
  YYSYMBOL_ON = 43,                        /* ON  */
  YYSYMBOL_GROUP = 44,                     /* GROUP  */
  YYSYMBOL_ORDER = 45,                     /* ORDER  */
  YYSYMBOL_BY = 46,                        /* BY  */
  YYSYMBOL_ASC = 47,                       /* ASC  */
  YYSYMBOL_DESC = 48,                      /* DESC  */
  YYSYMBOL_LOAD = 49,                      /* LOAD  */
  YYSYMBOL_DATA = 50,                      /* DATA  */
  YYSYMBOL_INFILE = 51,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 52,                   /* EXPLAIN  */
  YYSYMBOL_MAX = 53,                       /* MAX  */
  YYSYMBOL_MIN = 54,                       /* MIN  */
  YYSYMBOL_AVG = 55,                       /* AVG  */
  YYSYMBOL_COUNT = 56,                     /* COUNT  */
  YYSYMBOL_SUM = 57,                       /* SUM  */
  YYSYMBOL_LENGTH = 58,                    /* LENGTH  */
  YYSYMBOL_ROUND = 59,                     /* ROUND  */
  YYSYMBOL_DATE_FORMAT = 60,               /* DATE_FORMAT  */
  YYSYMBOL_EXISTS = 61,                    /* EXISTS  */
  YYSYMBOL_IN = 62,                        /* IN  */
  YYSYMBOL_EQ = 63,                        /* EQ  */
  YYSYMBOL_LT = 64,                        /* LT  */
  YYSYMBOL_GT = 65,                        /* GT  */
  YYSYMBOL_LE = 66,                        /* LE  */
  YYSYMBOL_GE = 67,                        /* GE  */
  YYSYMBOL_NE = 68,                        /* NE  */
  YYSYMBOL_LIKE = 69,                      /* LIKE  */
  YYSYMBOL_AS = 70,                        /* AS  */
  YYSYMBOL_NUMBER = 71,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 72,                     /* FLOAT  */
  YYSYMBOL_ID = 73,                        /* ID  */
  YYSYMBOL_SSS = 74,                       /* SSS  */
  YYSYMBOL_75_ = 75,                       /* '+'  */
  YYSYMBOL_76_ = 76,                       /* '-'  */
  YYSYMBOL_77_ = 77,                       /* '*'  */
  YYSYMBOL_78_ = 78,                       /* '/'  */
  YYSYMBOL_UMINUS = 79,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 80,                  /* $accept  */
  YYSYMBOL_commands = 81,                  /* commands  */
  YYSYMBOL_command_wrapper = 82,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 83,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 84,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 85,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 86,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 87,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 88,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 89,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 90,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 91,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 92,         /* create_index_stmt  */
  YYSYMBOL_idx_attr_list = 93,             /* idx_attr_list  */
  YYSYMBOL_drop_index_stmt = 94,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 95,         /* create_table_stmt  */
  YYSYMBOL_create_table_select = 96,       /* create_table_select  */
  YYSYMBOL_attr_def_list = 97,             /* attr_def_list  */
  YYSYMBOL_attr_def = 98,                  /* attr_def  */
  YYSYMBOL_number = 99,                    /* number  */
  YYSYMBOL_type = 100,                     /* type  */
  YYSYMBOL_insert_stmt = 101,              /* insert_stmt  */
  YYSYMBOL_value_lists = 102,              /* value_lists  */
  YYSYMBOL_value_list = 103,               /* value_list  */
  YYSYMBOL_value = 104,                    /* value  */
  YYSYMBOL_delete_stmt = 105,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 106,              /* update_stmt  */
  YYSYMBOL_set_clause_list = 107,          /* set_clause_list  */
  YYSYMBOL_set_clause = 108,               /* set_clause  */
  YYSYMBOL_select_stmt = 109,              /* select_stmt  */
  YYSYMBOL_calc_stmt = 110,                /* calc_stmt  */
  YYSYMBOL_simple_func_item = 111,         /* simple_func_item  */
  YYSYMBOL_simple_func_list = 112,         /* simple_func_list  */
  YYSYMBOL_expression_list = 113,          /* expression_list  */
  YYSYMBOL_expression = 114,               /* expression  */
  YYSYMBOL_select_attr = 115,              /* select_attr  */
  YYSYMBOL_alias = 116,                    /* alias  */
  YYSYMBOL_rel_attr = 117,                 /* rel_attr  */
  YYSYMBOL_sub_query = 118,                /* sub_query  */
  YYSYMBOL_attr_list = 119,                /* attr_list  */
  YYSYMBOL_rel_list = 120,                 /* rel_list  */
  YYSYMBOL_rel_condition_list = 121,       /* rel_condition_list  */
  YYSYMBOL_where = 122,                    /* where  */
  YYSYMBOL_having = 123,                   /* having  */
  YYSYMBOL_condition_list = 124,           /* condition_list  */
  YYSYMBOL_condition = 125,                /* condition  */
  YYSYMBOL_group = 126,                    /* group  */
  YYSYMBOL_group_list = 127,               /* group_list  */
  YYSYMBOL_group_item = 128,               /* group_item  */
  YYSYMBOL_order = 129,                    /* order  */
  YYSYMBOL_order_condition_list = 130,     /* order_condition_list  */
  YYSYMBOL_order_condition = 131,          /* order_condition  */
  YYSYMBOL_agg_func = 132,                 /* agg_func  */
  YYSYMBOL_func = 133,                     /* func  */
  YYSYMBOL_comp_op = 134,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 135,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 136,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 137,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 138             /* opt_semicolon  */
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
typedef yytype_int16 yy_state_t;

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  83
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   408

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  80
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  168
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  335

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   330


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    77,    75,     2,    76,     2,    78,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      79
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   247,   247,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   278,   284,   289,   295,   301,   307,   313,
     320,   326,   334,   351,   373,   376,   388,   398,   416,   423,
     442,   455,   471,   474,   487,   496,   509,   513,   519,   522,
     523,   524,   525,   526,   529,   552,   578,   581,   598,   620,
     623,   632,   644,   648,   652,   657,   665,   677,   693,   703,
     711,   719,   728,   738,   777,   794,   804,   829,   860,   863,
     876,   881,   892,   895,   898,   901,   904,   908,   911,   916,
     921,   927,   933,   940,   945,   950,   968,   991,   998,  1008,
    1023,  1026,  1029,  1035,  1070,  1075,  1088,  1127,  1130,  1140,
    1156,  1159,  1173,  1177,  1189,  1209,  1212,  1217,  1220,  1226,
    1229,  1235,  1242,  1251,  1260,  1293,  1296,  1308,  1311,  1322,
    1327,  1337,  1340,  1351,  1354,  1366,  1372,  1380,  1386,  1392,
    1400,  1409,  1410,  1411,  1412,  1413,  1416,  1417,  1418,  1421,
    1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,
    1432,  1433,  1434,  1438,  1451,  1459,  1467,  1477,  1478
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
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT", "SHOW", "SYNC",
  "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "DATE_T",
  "TEXT_T", "null", "IS", "NOT", "HELP", "EXIT", "DOT", "INTO", "VALUES",
  "FROM", "WHERE", "HAVING", "AND", "OR", "SET", "INNER", "JOIN", "ON",
  "GROUP", "ORDER", "BY", "ASC", "DESC", "LOAD", "DATA", "INFILE",
  "EXPLAIN", "MAX", "MIN", "AVG", "COUNT", "SUM", "LENGTH", "ROUND",
  "DATE_FORMAT", "EXISTS", "IN", "EQ", "LT", "GT", "LE", "GE", "NE",
  "LIKE", "AS", "NUMBER", "FLOAT", "ID", "SSS", "'+'", "'-'", "'*'", "'/'",
  "UMINUS", "$accept", "commands", "command_wrapper", "exit_stmt",
  "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "idx_attr_list", "drop_index_stmt",
  "create_table_stmt", "create_table_select", "attr_def_list", "attr_def",
  "number", "type", "insert_stmt", "value_lists", "value_list", "value",
  "delete_stmt", "update_stmt", "set_clause_list", "set_clause",
  "select_stmt", "calc_stmt", "simple_func_item", "simple_func_list",
  "expression_list", "expression", "select_attr", "alias", "rel_attr",
  "sub_query", "attr_list", "rel_list", "rel_condition_list", "where",
  "having", "condition_list", "condition", "group", "group_list",
  "group_item", "order", "order_condition_list", "order_condition",
  "agg_func", "func", "comp_op", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-276)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-60)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     333,     6,    63,   201,   134,    23,  -276,    -9,    14,   -29,
    -276,  -276,  -276,  -276,  -276,   -17,    32,    69,   333,    64,
     122,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,    62,    68,   121,    73,    75,   201,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,   107,  -276,
     201,  -276,  -276,    56,   -50,   201,   126,   117,  -276,    33,
      81,   116,    33,  -276,   201,  -276,    79,    87,   124,    99,
    -276,   115,  -276,  -276,  -276,  -276,    12,   125,    94,  -276,
     127,   -10,    98,  -276,   201,   201,   201,   201,   201,  -276,
       9,   155,   156,    21,   252,     1,   -31,   102,  -276,   158,
     123,   185,    45,   170,   176,   146,   142,   141,   148,   216,
    -276,  -276,   157,   186,   159,  -276,  -276,  -276,    17,    17,
    -276,  -276,  -276,   161,  -276,  -276,  -276,   201,    33,   151,
     214,   185,  -276,    29,  -276,    33,   260,  -276,  -276,    66,
     219,   193,  -276,   175,   176,   223,    66,  -276,   212,   266,
      16,  -276,   254,   198,  -276,   261,    38,   245,    66,   265,
    -276,  -276,    33,    66,    -5,   253,    33,   276,   168,   193,
     234,  -276,    71,     4,  -276,   146,  -276,   288,  -276,  -276,
    -276,  -276,  -276,   305,   148,  -276,   277,   310,   249,   313,
    -276,  -276,   257,   265,  -276,   158,    45,   262,   297,   176,
     -26,   185,  -276,    66,   265,  -276,   311,    -8,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,   252,   193,   193,
     331,    66,  -276,  -276,  -276,   278,   279,    16,  -276,    15,
     337,   283,    33,  -276,  -276,    33,   284,   314,   185,  -276,
     265,   332,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,   342,  -276,  -276,   287,   344,   337,    -5,    -5,    33,
     316,   320,  -276,   349,   350,  -276,   351,  -276,   353,   176,
    -276,   193,   294,   325,   335,   350,   358,  -276,   302,  -276,
    -276,   314,    -5,   345,   360,   303,   193,  -276,  -276,   210,
     351,   320,  -276,   306,   294,  -276,   -15,   362,  -276,    66,
     265,  -276,   335,  -276,   360,   315,  -276,  -276,   303,  -276,
     265,   366,   367,  -276,    80,   362,   369,   350,  -276,  -276,
    -276,  -276,   350,  -276,  -276
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
      26,    27,    28,    24,    23,     0,     0,     0,     0,     0,
     167,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,     0,    65,   141,   142,
     144,   143,   145,   146,   147,   148,    62,    63,    89,    64,
       0,    88,    75,    80,     0,     0,     0,    89,    97,   100,
     103,     0,   100,   104,     0,    30,     0,     0,     0,     0,
      31,     0,   164,     1,   168,     2,     0,     0,     0,    29,
       0,     0,     0,    87,     0,     0,     0,     0,     0,    48,
       0,     0,     0,     0,     0,    88,     0,     0,   101,    78,
       0,   107,    88,     0,   115,     0,     0,     0,     0,     0,
      38,    40,     0,     0,     0,    86,    90,    81,    82,    83,
      84,    85,    91,     0,    93,    94,    95,     0,   100,     0,
       0,   107,   102,     0,    74,   100,     0,    98,    76,     0,
       0,   119,    66,     0,   115,    69,     0,   165,     0,     0,
      42,    41,     0,     0,    36,     0,     0,     0,     0,    59,
     105,    99,   100,     0,   112,    89,   100,     0,     0,   119,
       0,   116,   120,     0,    67,     0,   166,     0,    49,    50,
      51,    52,    53,    45,     0,    47,     0,     0,     0,     0,
      92,    96,     0,    59,    60,    78,     0,     0,     0,   115,
       0,   107,    77,     0,    59,   123,   155,     0,   157,   159,
     149,   150,   151,   152,   153,   154,   161,     0,   119,   119,
       0,     0,    70,    72,    68,     0,     0,    42,    46,    37,
      34,     0,   100,    61,    79,   100,     0,   125,   107,   108,
      59,     0,   156,   158,   160,   162,   124,   121,   122,    71,
     163,     0,    43,    39,     0,     0,    34,   112,   112,   100,
       0,   131,   109,     0,    56,    44,   110,    32,     0,   115,
     113,   119,     0,     0,   117,    56,     0,    54,     0,    35,
      33,   125,   112,   129,   127,     0,   119,    73,    55,     0,
     110,   131,   114,     0,     0,   126,   135,   133,   118,     0,
      59,   111,   117,   130,   127,     0,   137,   138,     0,   132,
      59,     0,     0,   128,   136,   133,     0,    56,   106,   139,
     140,   134,    56,    57,    58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -276,  -276,   371,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,   128,  -276,  -276,   152,   150,   196,   160,
    -276,  -276,  -275,  -166,   -66,  -276,  -276,   207,  -276,   -84,
    -276,   250,   190,   304,    26,  -276,   -68,    -3,   217,  -130,
      97,  -252,  -149,    89,  -170,  -276,   108,    88,   100,   104,
      78,    90,  -276,     2,  -276,  -276,  -276,  -276,  -276
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   265,    31,    32,   120,   197,   160,   102,
     193,    33,   287,   140,    61,    34,    35,   154,   155,    36,
      37,    69,   144,    62,    70,    71,   109,   180,    73,   147,
     289,   209,   152,   297,   181,   182,   271,   305,   294,   284,
     319,   307,    64,    65,   227,    38,    39,    40,    85
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     105,    72,   121,   204,   111,   184,    74,   125,   112,   215,
     298,   171,    41,   207,    42,   279,   280,   315,   -59,   139,
     230,    99,     4,   100,    76,     4,   132,   101,   118,    63,
      75,    47,   316,   317,   194,   161,   208,   243,   136,   137,
     302,   133,   126,   195,    78,   196,   141,   126,   251,    77,
     157,   248,   333,   253,   254,   201,    79,   334,   257,   258,
     247,   255,   148,   149,    83,    95,    96,    97,    98,    44,
     167,    45,    91,   169,    94,    56,    57,   174,    59,    43,
     231,   249,   119,   177,   273,   119,    93,    53,    54,    55,
     186,   103,    91,    47,    97,    98,    95,    96,    97,    98,
     103,   138,   203,   107,   205,    80,   108,   206,   211,   228,
     229,   292,   214,    95,    96,    97,    98,   232,   272,    81,
      63,   128,   129,   130,   131,    84,   308,   329,   330,    88,
     291,    95,    96,    97,    98,    86,   104,    56,    57,    92,
      59,    87,    46,   176,   321,   173,    89,   250,    90,   106,
      66,   110,   113,    47,   326,   121,    95,    96,    97,    98,
     114,    47,   116,   166,   115,   259,   117,   123,   122,    47,
     124,   126,   134,   135,   267,   142,   143,   268,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    48,    49,    50,
      51,    52,    53,    54,    55,    47,   145,    56,    57,    58,
      59,   281,    60,   146,   150,    56,    57,    67,    59,    66,
      60,    68,   151,    56,    57,   158,    59,    46,   156,   153,
      47,   159,    56,    57,   256,    59,     4,   168,    47,   163,
     162,   170,   164,   310,   165,   178,   179,    47,   183,    56,
      57,   185,    59,   320,   213,   187,    48,    49,    50,    51,
      52,    53,    54,    55,    48,    49,    50,    51,    52,    53,
      54,    55,   216,   217,    56,    57,    58,    59,    66,    60,
     198,   199,    56,    57,    58,    59,    66,    60,   200,    47,
     202,    56,    57,   139,    59,   210,   309,    47,   188,   189,
     190,   191,   192,   212,   235,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   238,    48,    49,    50,    51,    52,
      53,    54,    55,    48,    49,    50,    51,    52,    53,    54,
      55,   236,   240,    56,    57,    58,    59,   239,    60,   241,
     242,    56,    57,   175,    59,   245,    60,     1,     2,   246,
     252,   104,     3,     4,     5,     6,     7,     8,     9,   274,
      99,   260,    10,    11,    12,   264,   266,   269,   270,   275,
     276,   277,   282,    13,    14,   283,   285,   293,   286,   288,
     290,   295,   296,    15,   299,   300,   306,   303,   304,   313,
     318,    16,    17,   327,   328,    18,   332,   262,   324,    82,
     237,   263,   234,   172,   278,   244,   261,   311,   127,   301,
     233,   322,   323,   331,   314,   312,     0,     0,   325
};

static const yytype_int16 yycheck[] =
{
      66,     4,    86,   169,    72,   154,     4,    17,    74,   179,
     285,   141,     6,    18,     8,   267,   268,    32,    17,    18,
      16,    71,    10,    73,    33,    10,    17,    77,    16,     3,
       7,    27,    47,    48,    18,   119,    41,   203,    17,    18,
     292,    32,    73,    27,    73,    29,    77,    73,   214,    35,
     116,    77,   327,    61,    62,    17,    73,   332,   228,   229,
     209,    69,    17,    18,     0,    75,    76,    77,    78,     6,
     138,     8,    46,   139,    18,    71,    72,   145,    74,    73,
      76,   211,    70,   149,   250,    70,    60,    58,    59,    60,
     156,    65,    66,    27,    77,    78,    75,    76,    77,    78,
      74,   104,   168,    70,   172,    73,    73,   173,   176,    38,
      39,   281,   178,    75,    76,    77,    78,   183,   248,    50,
      94,    95,    96,    97,    98,     3,   296,    47,    48,     8,
     279,    75,    76,    77,    78,    73,    10,    71,    72,    32,
      74,    73,    16,   146,   310,   143,    73,   213,    73,    32,
      16,    35,    73,    27,   320,   239,    75,    76,    77,    78,
      73,    27,    63,   137,    40,   231,    51,    73,    43,    27,
      43,    73,    17,    17,   242,    73,    18,   245,    27,    53,
      54,    55,    56,    57,    58,    59,    60,    53,    54,    55,
      56,    57,    58,    59,    60,    27,    73,    71,    72,    73,
      74,   269,    76,    18,    34,    71,    72,    73,    74,    16,
      76,    77,    36,    71,    72,    74,    74,    16,    76,    73,
      27,    73,    71,    72,   227,    74,    10,    76,    27,    43,
      73,    17,    73,   299,    73,    16,    43,    27,    63,    71,
      72,    18,    74,   309,    76,    33,    53,    54,    55,    56,
      57,    58,    59,    60,    53,    54,    55,    56,    57,    58,
      59,    60,    28,    29,    71,    72,    73,    74,    16,    76,
      16,    73,    71,    72,    73,    74,    16,    76,    17,    27,
      35,    71,    72,    18,    74,    32,    76,    27,    22,    23,
      24,    25,    26,    17,     6,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    27,    53,    54,    55,    56,    57,
      58,    59,    60,    53,    54,    55,    56,    57,    58,    59,
      60,    16,    73,    71,    72,    73,    74,    17,    76,    16,
      73,    71,    72,    73,    74,    73,    76,     4,     5,    42,
      29,    10,     9,    10,    11,    12,    13,    14,    15,    17,
      71,    73,    19,    20,    21,    18,    73,    73,    44,    17,
      73,    17,    46,    30,    31,    45,    17,    73,    18,    18,
      17,    46,    37,    40,    16,    73,    73,    32,    18,    73,
      18,    48,    49,    17,    17,    52,    17,   237,    73,    18,
     194,   239,   185,   143,   266,   205,   236,   300,    94,   291,
     183,   312,   314,   325,   304,   301,    -1,    -1,   318
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     9,    10,    11,    12,    13,    14,    15,
      19,    20,    21,    30,    31,    40,    48,    49,    52,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    94,    95,   101,   105,   106,   109,   110,   135,   136,
     137,     6,     8,    73,     6,     8,    16,    27,    53,    54,
      55,    56,    57,    58,    59,    60,    71,    72,    73,    74,
      76,   104,   113,   114,   132,   133,    16,    73,    77,   111,
     114,   115,   117,   118,   133,     7,    33,    35,    73,    73,
      73,    50,    82,     0,     3,   138,    73,    73,     8,    73,
      73,   114,    32,   114,    18,    75,    76,    77,    78,    71,
      73,    77,    99,   114,    10,   104,    32,    70,    73,   116,
      35,   116,   104,    73,    73,    40,    63,    51,    16,    70,
      96,   109,    43,    73,    43,    17,    73,   113,   114,   114,
     114,   114,    17,    32,    17,    17,    17,    18,   117,    18,
     103,    77,    73,    18,   112,    73,    18,   119,    17,    18,
      34,    36,   122,    73,   107,   108,    76,   104,    74,    73,
      98,   109,    73,    43,    73,    73,   114,   116,    76,   104,
      17,   119,   111,   133,   116,    73,   117,   104,    16,    43,
     117,   124,   125,    63,   122,    18,   104,    33,    22,    23,
      24,    25,    26,   100,    18,    27,    29,    97,    16,    73,
      17,    17,    35,   104,   103,   116,   104,    18,    41,   121,
      32,   116,    17,    76,   104,   124,    28,    29,    61,    62,
      63,    64,    65,    66,    67,    68,    69,   134,    38,    39,
      16,    76,   104,   118,   107,     6,    16,    98,    27,    17,
      73,    16,    73,   103,   112,    73,    42,   122,    77,   119,
     104,   103,    29,    61,    62,    69,   117,   124,   124,   104,
      73,    99,    97,    96,    18,    93,    73,   116,   116,    73,
      44,   126,   119,   103,    17,    17,    73,    17,    93,   121,
     121,   116,    46,    45,   129,    17,    18,   102,    18,   120,
      17,   122,   124,    73,   128,    46,    37,   123,   102,    16,
      73,   126,   121,    32,    18,   127,    73,   131,   124,    76,
     104,   120,   129,    73,   128,    32,    47,    48,    18,   130,
     104,   103,   123,   127,    73,   131,   103,    17,    17,    47,
      48,   130,    17,   102,   102
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    80,    81,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    92,    93,    93,    94,    95,    95,    95,
      96,    96,    97,    97,    98,    98,    98,    98,    99,   100,
     100,   100,   100,   100,   101,   101,   102,   102,   102,   103,
     103,   103,   104,   104,   104,   104,   105,   106,   107,   107,
     108,   108,   108,   109,   109,   110,   111,   111,   112,   112,
     113,   113,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   115,   115,   115,
     116,   116,   116,   117,   117,   117,   118,   119,   119,   119,
     120,   120,   121,   121,   121,   122,   122,   123,   123,   124,
     124,   124,   124,   124,   125,   126,   126,   127,   127,   128,
     128,   129,   129,   130,   130,   131,   131,   131,   131,   131,
     131,   132,   132,   132,   132,   132,   133,   133,   133,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   135,   136,   137,   137,   138,   138
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     9,    10,     0,     3,     5,     7,     4,     8,
       1,     2,     0,     3,     5,     2,     3,     2,     1,     1,
       1,     1,     1,     1,     9,    10,     0,     6,     7,     0,
       3,     4,     1,     1,     1,     1,     4,     5,     3,     1,
       3,     4,     3,    10,     4,     2,     3,     5,     0,     4,
       1,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       3,     3,     5,     3,     3,     3,     5,     1,     3,     4,
       0,     1,     2,     1,     1,     4,    13,     0,     4,     5,
       0,     3,     0,     4,     6,     0,     2,     0,     2,     0,
       1,     3,     3,     2,     3,     0,     4,     0,     3,     1,
       3,     0,     4,     0,     3,     1,     3,     2,     2,     4,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     1,     2,     7,     2,     4,     5,     0,     1
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
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
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
  YYLTYPE *yylloc;
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 248 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1913 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 278 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1922 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 284 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1930 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 289 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1938 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 295 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1946 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 301 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1954 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 307 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1962 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 313 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1972 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 320 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1980 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 326 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1990 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID idx_attr_list RBRACE  */
#line 335 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      if ((yyvsp[-1].index_attr_list) != nullptr) {
        create_index.attribute_names.swap(*(yyvsp[-1].index_attr_list));
        delete (yyvsp[-1].index_attr_list);
      }
      create_index.attribute_names.push_back((yyvsp[-2].string));
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      create_index.index_type = "NORMAL";
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2011 "yacc_sql.cpp"
    break;

  case 33: /* create_index_stmt: CREATE ID INDEX ID ON ID LBRACE ID idx_attr_list RBRACE  */
#line 352 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      if ((yyvsp[-1].index_attr_list) != nullptr) {
        create_index.attribute_names.swap(*(yyvsp[-1].index_attr_list));
        delete (yyvsp[-1].index_attr_list);
      }
      create_index.attribute_names.push_back((yyvsp[-2].string));
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      create_index.index_type = (yyvsp[-8].string);
      free((yyvsp[-8].string));
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2033 "yacc_sql.cpp"
    break;

  case 34: /* idx_attr_list: %empty  */
#line 373 "yacc_sql.y"
    {
      (yyval.index_attr_list) = nullptr;
    }
#line 2041 "yacc_sql.cpp"
    break;

  case 35: /* idx_attr_list: COMMA ID rel_list  */
#line 376 "yacc_sql.y"
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.index_attr_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.index_attr_list) = new std::vector<std::string>;
      }
      (yyval.index_attr_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2055 "yacc_sql.cpp"
    break;

  case 36: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 389 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2067 "yacc_sql.cpp"
    break;

  case 37: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 399 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-4].string);
      free((yyvsp[-4].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-1].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-2].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-2].attr_info);
      delete (yyvsp[-1].attr_infos);
    }
#line 2088 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID create_table_select  */
#line 416 "yacc_sql.y"
                                          {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      create_table.select_table.reset((yyvsp[0].select_sql_node));
    }
#line 2100 "yacc_sql.cpp"
    break;

  case 39: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE create_table_select  */
#line 423 "yacc_sql.y"
                                                                               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));
      create_table.select_table.reset((yyvsp[0].select_sql_node));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);
      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      delete (yyvsp[-2].attr_infos);
    }
#line 2121 "yacc_sql.cpp"
    break;

  case 40: /* create_table_select: select_stmt  */
#line 443 "yacc_sql.y"
  {
      (yyval.select_sql_node) = new SelectSqlNode;
      SelectSqlNode &select_sql_node = (yyvsp[0].sql_node)->selection;
      (yyval.select_sql_node)->attributes.swap(select_sql_node.attributes);
      (yyval.select_sql_node)->relations.swap(select_sql_node.relations);
      (yyval.select_sql_node)->join_conditions.swap(select_sql_node.join_conditions);
      (yyval.select_sql_node)->conditions.swap(select_sql_node.conditions);
      (yyval.select_sql_node)->groups.swap(select_sql_node.groups);
      (yyval.select_sql_node)->orders.swap(select_sql_node.orders);
      (yyval.select_sql_node)->havings.swap(select_sql_node.havings);
      delete (yyvsp[0].sql_node);
  }
#line 2138 "yacc_sql.cpp"
    break;

  case 41: /* create_table_select: AS select_stmt  */
#line 456 "yacc_sql.y"
  {
      (yyval.select_sql_node) = new SelectSqlNode;
      SelectSqlNode &select_sql_node = (yyvsp[0].sql_node)->selection;
      (yyval.select_sql_node)->attributes.swap(select_sql_node.attributes);
      (yyval.select_sql_node)->relations.swap(select_sql_node.relations);
      (yyval.select_sql_node)->join_conditions.swap(select_sql_node.join_conditions);
      (yyval.select_sql_node)->conditions.swap(select_sql_node.conditions);
      (yyval.select_sql_node)->groups.swap(select_sql_node.groups);
      (yyval.select_sql_node)->orders.swap(select_sql_node.orders);
      (yyval.select_sql_node)->havings.swap(select_sql_node.havings);
      delete (yyvsp[0].sql_node);
  }
#line 2155 "yacc_sql.cpp"
    break;

  case 42: /* attr_def_list: %empty  */
#line 471 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2163 "yacc_sql.cpp"
    break;

  case 43: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 475 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2177 "yacc_sql.cpp"
    break;

  case 44: /* attr_def: ID type LBRACE number RBRACE  */
#line 488 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      (yyval.attr_info)->not_null = true;
      free((yyvsp[-4].string));
    }
#line 2190 "yacc_sql.cpp"
    break;

  case 45: /* attr_def: ID type  */
#line 497 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      if((yyval.attr_info)->type == TEXTS) {
        (yyval.attr_info)->length = 65535;
      } else{
        (yyval.attr_info)->length = 4;
      }
      (yyval.attr_info)->not_null = true;
      free((yyvsp[-1].string));
    }
#line 2207 "yacc_sql.cpp"
    break;

  case 46: /* attr_def: attr_def NOT null  */
#line 509 "yacc_sql.y"
                        {
      (yyval.attr_info) = (yyvsp[-2].attr_info);
      (yyval.attr_info)->not_null = true;
    }
#line 2216 "yacc_sql.cpp"
    break;

  case 47: /* attr_def: attr_def null  */
#line 513 "yacc_sql.y"
                    {
      (yyval.attr_info) = (yyvsp[-1].attr_info);
      (yyval.attr_info)->not_null = false;
    }
#line 2225 "yacc_sql.cpp"
    break;

  case 48: /* number: NUMBER  */
#line 519 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2231 "yacc_sql.cpp"
    break;

  case 49: /* type: INT_T  */
#line 522 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2237 "yacc_sql.cpp"
    break;

  case 50: /* type: STRING_T  */
#line 523 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2243 "yacc_sql.cpp"
    break;

  case 51: /* type: FLOAT_T  */
#line 524 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2249 "yacc_sql.cpp"
    break;

  case 52: /* type: DATE_T  */
#line 525 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2255 "yacc_sql.cpp"
    break;

  case 53: /* type: TEXT_T  */
#line 526 "yacc_sql.y"
               { (yyval.number)=TEXTS; }
#line 2261 "yacc_sql.cpp"
    break;

  case 54: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE value_lists  */
#line 530 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-6].string);
      if ((yyvsp[0].value_lists) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[0].value_lists));
      }
      if ((yyvsp[-2].value_list) != nullptr) {
        std::vector<Value> *tmp = (yyvsp[-2].value_list);
        tmp->emplace_back(*(yyvsp[-3].value));
        (yyval.sql_node)->insertion.values.emplace_back(*tmp);
        delete (yyvsp[-2].value_list);
      }else {
        std::vector<Value> tmp = {*(yyvsp[-3].value)};
        (yyval.sql_node)->insertion.values.emplace_back(tmp);
      }
      for (auto &value : (yyval.sql_node)->insertion.values) {
        std::reverse(value.begin(), value.end());
      }
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-3].value);
      free((yyvsp[-6].string));
    }
#line 2288 "yacc_sql.cpp"
    break;

  case 55: /* insert_stmt: INSERT INTO ID VALUES LBRACE '-' value value_list RBRACE value_lists  */
#line 552 "yacc_sql.y"
                                                                           {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-7].string);
      if ((yyvsp[0].value_lists) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[0].value_lists));
      }
      modify_2_negative((yyvsp[-3].value));
      if ((yyvsp[-2].value_list) != nullptr) {
        std::vector<Value> *tmp = (yyvsp[-2].value_list);
        tmp->emplace_back(*(yyvsp[-3].value));
        (yyval.sql_node)->insertion.values.emplace_back(*tmp);
        delete (yyvsp[-2].value_list);
      }else {
        std::vector<Value> tmp = {*(yyvsp[-3].value)};
        (yyval.sql_node)->insertion.values.emplace_back(tmp);
      }
      for (auto &value : (yyval.sql_node)->insertion.values) {
        std::reverse(value.begin(), value.end());
      }
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-3].value);
      free((yyvsp[-7].string));
    }
#line 2316 "yacc_sql.cpp"
    break;

  case 56: /* value_lists: %empty  */
#line 578 "yacc_sql.y"
  {
    (yyval.value_lists) = nullptr;
  }
#line 2324 "yacc_sql.cpp"
    break;

  case 57: /* value_lists: COMMA LBRACE value value_list RBRACE value_lists  */
#line 581 "yacc_sql.y"
                                                     {
    if ((yyvsp[0].value_lists) != nullptr) {
      (yyval.value_lists) = (yyvsp[0].value_lists);
    }else {
      (yyval.value_lists) = new std::vector<std::vector<Value>>;
    }
    if ((yyvsp[-2].value_list) != nullptr) {
      std::vector<Value> *tmp = (yyvsp[-2].value_list);
      tmp->emplace_back(*(yyvsp[-3].value));
      (yyval.value_lists)->emplace_back(*(yyvsp[-2].value_list));
      delete (yyvsp[-2].value_list);
    }else {
      std::vector<Value> tmp = {*(yyvsp[-3].value)};
      (yyval.value_lists)->emplace_back(tmp);
    }
    delete (yyvsp[-3].value);
  }
#line 2346 "yacc_sql.cpp"
    break;

  case 58: /* value_lists: COMMA LBRACE '-' value value_list RBRACE value_lists  */
#line 598 "yacc_sql.y"
                                                         {
    if ((yyvsp[0].value_lists) != nullptr) {
      (yyval.value_lists) = (yyvsp[0].value_lists);
    }else {
      (yyval.value_lists) = new std::vector<std::vector<Value>>;
    }
    modify_2_negative((yyvsp[-3].value));
    if ((yyvsp[-2].value_list) != nullptr) {
      std::vector<Value> *tmp = (yyvsp[-2].value_list);
      tmp->emplace_back(*(yyvsp[-3].value));
      (yyval.value_lists)->emplace_back(*(yyvsp[-2].value_list));
      delete (yyvsp[-3].value);
    }else {
      std::vector<Value> tmp = {*(yyvsp[-3].value)};
      (yyval.value_lists)->emplace_back(tmp);
    }
    delete (yyvsp[-3].value);
  }
#line 2369 "yacc_sql.cpp"
    break;

  case 59: /* value_list: %empty  */
#line 620 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2377 "yacc_sql.cpp"
    break;

  case 60: /* value_list: COMMA value value_list  */
#line 623 "yacc_sql.y"
                             { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2391 "yacc_sql.cpp"
    break;

  case 61: /* value_list: COMMA '-' value value_list  */
#line 632 "yacc_sql.y"
                                 {
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      modify_2_negative((yyvsp[-1].value));
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2406 "yacc_sql.cpp"
    break;

  case 62: /* value: NUMBER  */
#line 644 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2415 "yacc_sql.cpp"
    break;

  case 63: /* value: FLOAT  */
#line 648 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2424 "yacc_sql.cpp"
    break;

  case 64: /* value: SSS  */
#line 652 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
#line 2434 "yacc_sql.cpp"
    break;

  case 65: /* value: null  */
#line 657 "yacc_sql.y"
          {
      (yyval.value) = new Value();
      (yyval.value)->set_null();
      (yyloc) = (yylsp[0]);
    }
#line 2444 "yacc_sql.cpp"
    break;

  case 66: /* delete_stmt: DELETE FROM ID where  */
#line 666 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2458 "yacc_sql.cpp"
    break;

  case 67: /* update_stmt: UPDATE ID SET set_clause_list where  */
#line 678 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-3].string);
      if ((yyvsp[-1].set_clause_list) != nullptr) {
        (yyval.sql_node)->update.set_clause_list.swap(*(yyvsp[-1].set_clause_list));
        delete (yyvsp[-1].set_clause_list);
      }     
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-3].string));
    }
#line 2476 "yacc_sql.cpp"
    break;

  case 68: /* set_clause_list: set_clause COMMA set_clause_list  */
#line 694 "yacc_sql.y"
    {
      if((yyvsp[0].set_clause_list) != nullptr) {
        (yyval.set_clause_list) = (yyvsp[0].set_clause_list);
        (yyval.set_clause_list)->push_back(*(yyvsp[-2].set_clause));
      } else {
        (yyval.set_clause_list) = new std::vector<SetClauseSqlNode>;
        (yyval.set_clause_list)->push_back(*(yyvsp[-2].set_clause));
      }
    }
#line 2490 "yacc_sql.cpp"
    break;

  case 69: /* set_clause_list: set_clause  */
#line 704 "yacc_sql.y"
    {
      (yyval.set_clause_list) = new std::vector<SetClauseSqlNode>;
      (yyval.set_clause_list)->push_back(*(yyvsp[0].set_clause));
    }
#line 2499 "yacc_sql.cpp"
    break;

  case 70: /* set_clause: ID EQ value  */
#line 712 "yacc_sql.y"
    {
      (yyval.set_clause) = new SetClauseSqlNode;
      (yyval.set_clause)->attribute_name_ = (yyvsp[-2].string);
      (yyval.set_clause)->value_ = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 2511 "yacc_sql.cpp"
    break;

  case 71: /* set_clause: ID EQ '-' value  */
#line 720 "yacc_sql.y"
    {
      (yyval.set_clause) = new SetClauseSqlNode;
      modify_2_negative((yyvsp[0].value));
      (yyval.set_clause)->attribute_name_ = (yyvsp[-3].string);
      (yyval.set_clause)->value_ = *(yyvsp[0].value);
      free((yyvsp[-3].string));
      delete (yyvsp[0].value);
    }
#line 2524 "yacc_sql.cpp"
    break;

  case 72: /* set_clause: ID EQ sub_query  */
#line 728 "yacc_sql.y"
                      {
      (yyval.set_clause) = new SetClauseSqlNode;
      (yyval.set_clause)->attribute_name_ = (yyvsp[-2].string);
      (yyval.set_clause)->sub_query_.reset((yyvsp[0].sub_select));
      free((yyvsp[-2].string));
    }
#line 2535 "yacc_sql.cpp"
    break;

  case 73: /* select_stmt: SELECT select_attr FROM ID alias rel_condition_list where group order having  */
#line 739 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-8].rel_attr_list) != nullptr) {
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[-8].rel_attr_list));
        delete (yyvsp[-8].rel_attr_list);
      }
      if ((yyvsp[-4].relationAndConditionTempList) != nullptr) {
        (yyval.sql_node)->selection.relations.swap((yyvsp[-4].relationAndConditionTempList)->_rel_list);
        (yyval.sql_node)->selection.join_conditions.swap((yyvsp[-4].relationAndConditionTempList)->_condition_list);
        std::reverse((yyval.sql_node)->selection.join_conditions.begin(), (yyval.sql_node)->selection.join_conditions.end());
        delete (yyvsp[-4].relationAndConditionTempList);
      }
      RelationSqlNode node;
      init_relation_sql_node((yyvsp[-6].string), (yyvsp[-5].string), node);
      (yyval.sql_node)->selection.relations.push_back(node);
      std::reverse((yyval.sql_node)->selection.relations.begin(), (yyval.sql_node)->selection.relations.end());
      if ((yyvsp[-3].condition_list) != nullptr) {
        std::reverse((yyvsp[-3].condition_list)->begin(), (yyvsp[-3].condition_list)->end());
        (yyval.sql_node)->selection.conditions.swap(*(yyvsp[-3].condition_list));
        delete (yyvsp[-3].condition_list);
      }
      if ((yyvsp[-2].rel_attr_list) != nullptr) {
        std::reverse((yyvsp[-2].rel_attr_list)->begin(), (yyvsp[-2].rel_attr_list)->end());
        (yyval.sql_node)->selection.groups.swap(*(yyvsp[-2].rel_attr_list));
        delete (yyvsp[-2].rel_attr_list);
      }
      if ((yyvsp[-1].order_condition_list) != nullptr) {
        std::reverse((yyvsp[-1].order_condition_list)->begin(), (yyvsp[-1].order_condition_list)->end());
        (yyval.sql_node)->selection.orders.swap(*(yyvsp[-1].order_condition_list));
        delete (yyvsp[-1].order_condition_list);
      }
      if ((yyvsp[0].condition_list) != nullptr) {
        std::reverse((yyvsp[0].condition_list)->begin(), (yyvsp[0].condition_list)->end());
        (yyval.sql_node)->selection.havings.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-6].string));
    }
#line 2578 "yacc_sql.cpp"
    break;

  case 74: /* select_stmt: SELECT simple_func_item alias simple_func_list  */
#line 777 "yacc_sql.y"
                                                                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      (yyvsp[-2].rel_attr)->alias.reset((yyvsp[-1].string));
      (yyvsp[-2].rel_attr)->expression->set_alias((yyvsp[-1].string));
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyvsp[0].rel_attr_list)->push_back(*(yyvsp[-2].rel_attr));
        std::reverse((yyvsp[0].rel_attr_list)->begin(), (yyvsp[0].rel_attr_list)->end());
        (yyval.sql_node)->selection.attributes.swap(*(yyvsp[0].rel_attr_list));
        delete (yyvsp[0].rel_attr_list);
      }else {
        (yyval.sql_node)->selection.attributes.push_back(*(yyvsp[-2].rel_attr));
      }
      delete (yyvsp[-2].rel_attr);
    }
#line 2597 "yacc_sql.cpp"
    break;

  case 75: /* calc_stmt: CALC expression_list  */
#line 795 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2608 "yacc_sql.cpp"
    break;

  case 76: /* simple_func_item: func value RBRACE  */
#line 804 "yacc_sql.y"
                    {
    // 这里的ValueExpr找不到合适的地方释放
    ValueExpr *value_expr = new ValueExpr(std::move(*(yyvsp[-1].value)));
    value_expr->set_name(value_expr->get_value().to_string());
    FuncExpr *expr = new FuncExpr((yyvsp[-2].func), 1, value_expr, nullptr);
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->expression = expr;
    (yyval.rel_attr)->func_type = (yyvsp[-2].func);
    std::string name;
    switch ((yyvsp[-2].func)) {
      case FUNC_LENGTH: {
        name += "LENGTH(";
      }break;
      case FUNC_ROUND: {
        name += "ROUND(";
      }break;
      case FUNC_DATE_FORMAT: {
        name += "DATE_FORMAT(";
      }break;
    }
    name += value_expr->name();
    name += ")";
    expr->set_name(name);
    delete (yyvsp[-1].value);
  }
#line 2638 "yacc_sql.cpp"
    break;

  case 77: /* simple_func_item: func value COMMA value RBRACE  */
#line 829 "yacc_sql.y"
                                  {
    auto first = new ValueExpr(std::move(*(yyvsp[-3].value)));
    first->set_name(first->get_value().to_string());
    auto second = new ValueExpr(std::move(*(yyvsp[-1].value)));
    second->set_name(second->get_value().to_string());
    FuncExpr *expr = new FuncExpr((yyvsp[-4].func), 2, first, second);
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->expression = expr;
    (yyval.rel_attr)->func_type = (yyvsp[-4].func);
    std::string name;
    switch ((yyvsp[-4].func)) {
      case FUNC_LENGTH: {
        name += "LENGTH(";
      }break;
      case FUNC_ROUND: {
        name += "ROUND(";
      }break;
      case FUNC_DATE_FORMAT: {
        name += "DATE_FORMAT(";
      }break;
    }
    name += first->name();
    name += ",";
    name += second->name();
    name += ")";
    expr->set_name(name);
    delete (yyvsp[-3].value);
  }
#line 2671 "yacc_sql.cpp"
    break;

  case 78: /* simple_func_list: %empty  */
#line 860 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 2679 "yacc_sql.cpp"
    break;

  case 79: /* simple_func_list: COMMA simple_func_item alias simple_func_list  */
#line 863 "yacc_sql.y"
                                                  {
    if ((yyvsp[0].rel_attr_list) != nullptr) {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }else {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }
    (yyvsp[-2].rel_attr)->alias.reset((yyvsp[-1].string));
    (yyvsp[-2].rel_attr)->expression->set_alias((yyvsp[-1].string));
    (yyval.rel_attr_list)->push_back(*(yyvsp[-2].rel_attr));
    delete (yyvsp[-2].rel_attr);
  }
#line 2695 "yacc_sql.cpp"
    break;

  case 80: /* expression_list: expression  */
#line 877 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2704 "yacc_sql.cpp"
    break;

  case 81: /* expression_list: expression COMMA expression_list  */
#line 882 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2717 "yacc_sql.cpp"
    break;

  case 82: /* expression: expression '+' expression  */
#line 892 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2725 "yacc_sql.cpp"
    break;

  case 83: /* expression: expression '-' expression  */
#line 895 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2733 "yacc_sql.cpp"
    break;

  case 84: /* expression: expression '*' expression  */
#line 898 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2741 "yacc_sql.cpp"
    break;

  case 85: /* expression: expression '/' expression  */
#line 901 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2749 "yacc_sql.cpp"
    break;

  case 86: /* expression: LBRACE expression RBRACE  */
#line 904 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2758 "yacc_sql.cpp"
    break;

  case 87: /* expression: '-' expression  */
#line 908 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2766 "yacc_sql.cpp"
    break;

  case 88: /* expression: value  */
#line 911 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2776 "yacc_sql.cpp"
    break;

  case 89: /* expression: ID  */
#line 916 "yacc_sql.y"
         {
      (yyval.expression) = new FieldExpr;
      (yyval.expression)->set_name(std::string((yyvsp[0].string)));
      free((yyvsp[0].string));
    }
#line 2786 "yacc_sql.cpp"
    break;

  case 90: /* expression: ID DOT ID  */
#line 921 "yacc_sql.y"
                {
      (yyval.expression) = new FieldExpr;
      (yyval.expression)->set_name(std::string((yyvsp[-2].string)).append(".").append(std::string((yyvsp[0].string))));
      free((yyvsp[0].string));
      free((yyvsp[-2].string));
    }
#line 2797 "yacc_sql.cpp"
    break;

  case 91: /* expression: agg_func ID RBRACE  */
#line 927 "yacc_sql.y"
                         {
      FieldExpr *field_expr = new FieldExpr;
      field_expr->set_name(std::string((yyvsp[-1].string)));
      (yyval.expression) = new AggrFuncExpr((yyvsp[-2].agg_func), field_expr);
      free((yyvsp[-1].string));
    }
#line 2808 "yacc_sql.cpp"
    break;

  case 92: /* expression: agg_func ID DOT ID RBRACE  */
#line 933 "yacc_sql.y"
                                {
      FieldExpr *field_expr = new FieldExpr;
      field_expr->set_name(std::string((yyvsp[-3].string)).append(".").append(std::string((yyvsp[-1].string))));
      (yyval.expression) = new AggrFuncExpr((yyvsp[-4].agg_func), field_expr);
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 2820 "yacc_sql.cpp"
    break;

  case 93: /* expression: agg_func '*' RBRACE  */
#line 940 "yacc_sql.y"
                          {
      ValueExpr *expr = new ValueExpr(Value("*", 2));
      expr->set_name("*");
      (yyval.expression) = new AggrFuncExpr((yyvsp[-2].agg_func), expr);
    }
#line 2830 "yacc_sql.cpp"
    break;

  case 94: /* expression: agg_func number RBRACE  */
#line 945 "yacc_sql.y"
                             {
      ValueExpr *expr = new ValueExpr(Value((yyvsp[-1].number)));
      expr->set_name(std::to_string((yyvsp[-1].number)));
      (yyval.expression) = new AggrFuncExpr((yyvsp[-2].agg_func), expr);
    }
#line 2840 "yacc_sql.cpp"
    break;

  case 95: /* expression: func expression RBRACE  */
#line 950 "yacc_sql.y"
                             {
      (yyval.expression) = new FuncExpr((yyvsp[-2].func), 1, (yyvsp[-1].expression), nullptr);
      std::string name;
      switch ((yyvsp[-2].func)) {
        case FUNC_LENGTH: {
          name += "LENGTH(";
        }break;
        case FUNC_ROUND: {
          name += "ROUND(";
        }break;
        case FUNC_DATE_FORMAT: {
          name += "DATE_FORMAT(";
        }break;
      }
      name += (yyvsp[-1].expression)->name();
      name += ")";
      (yyval.expression)->set_name(name);
    }
#line 2863 "yacc_sql.cpp"
    break;

  case 96: /* expression: func expression COMMA expression RBRACE  */
#line 968 "yacc_sql.y"
                                              {
      (yyval.expression) = new FuncExpr((yyvsp[-4].func), 2, (yyvsp[-3].expression), (yyvsp[-1].expression));
      std::string name;
      switch ((yyvsp[-4].func)) {
        case FUNC_LENGTH: {
          name += "LENGTH(";
        }break;
        case FUNC_ROUND: {
          name += "ROUND(";
        }break;
        case FUNC_DATE_FORMAT: {
          name += "DATE_FORMAT(";
        }break;
      }
      name += (yyvsp[-3].expression)->name();
      name += ", ";
      name += (yyvsp[-1].expression)->name();
      name += ")";
      (yyval.expression)->set_name(name);
    }
#line 2888 "yacc_sql.cpp"
    break;

  case 97: /* select_attr: '*'  */
#line 991 "yacc_sql.y"
        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      RelAttrSqlNode attr;
      attr.relation_name  = "";
      attr.attribute_name = "*";
      (yyval.rel_attr_list)->emplace_back(attr);
    }
#line 2900 "yacc_sql.cpp"
    break;

  case 98: /* select_attr: rel_attr alias attr_list  */
#line 998 "yacc_sql.y"
                               {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->alias.reset((yyvsp[-1].string));
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 2915 "yacc_sql.cpp"
    break;

  case 99: /* select_attr: ID DOT '*' attr_list  */
#line 1008 "yacc_sql.y"
                           {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      }else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      RelAttrSqlNode node;
      node.relation_name = (yyvsp[-3].string);
      node.attribute_name = "*";
      free((yyvsp[-3].string));
    }
#line 2931 "yacc_sql.cpp"
    break;

  case 100: /* alias: %empty  */
#line 1023 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2939 "yacc_sql.cpp"
    break;

  case 101: /* alias: ID  */
#line 1026 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2947 "yacc_sql.cpp"
    break;

  case 102: /* alias: AS ID  */
#line 1029 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2955 "yacc_sql.cpp"
    break;

  case 103: /* rel_attr: expression  */
#line 1035 "yacc_sql.y"
               {
      (yyval.rel_attr) = new RelAttrSqlNode;
      if ((yyvsp[0].expression)->type() == ExprType::FIELD) {
        std::string field_name = (yyvsp[0].expression)->name();
        std::string table_name;
        int idx = field_name.find_first_of(".");
        if (std::string::npos != idx) {
          (yyval.rel_attr)->relation_name = field_name.substr(0, idx);
          (yyval.rel_attr)->attribute_name = field_name.substr(idx + 1, field_name.size() - idx - 1);
        }else {
          (yyval.rel_attr)->attribute_name = (yyvsp[0].expression)->name();
        }
        delete (yyvsp[0].expression);
      }else if((yyvsp[0].expression)->type() == ExprType::AGGRFUNCTION){
        // TODO 降低冗余
        FieldExpr *expr = ((AggrFuncExpr *)(yyvsp[0].expression))->field();
        if (expr) {
          std::string field_name = expr->name();
          std::string table_name;
          int idx = field_name.find_first_of(".");
          if (std::string::npos != idx) {
            (yyval.rel_attr)->relation_name = field_name.substr(0, idx);
            (yyval.rel_attr)->attribute_name = field_name.substr(idx + 1, field_name.size() - idx - 1);
          }else {
            (yyval.rel_attr)->attribute_name = std::move(field_name);
          }
        }else {
          (yyval.rel_attr)->attribute_name = ((AggrFuncExpr *)(yyvsp[0].expression))->value()->name();
        }
        (yyval.rel_attr)->type = ((AggrFuncExpr *)(yyvsp[0].expression))->aggr_type();
      }else {
        (yyval.rel_attr)->expression = (yyvsp[0].expression);
      }
    }
#line 2994 "yacc_sql.cpp"
    break;

  case 104: /* rel_attr: sub_query  */
#line 1070 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->sub_query.reset((yyvsp[0].sub_select));
    }
#line 3003 "yacc_sql.cpp"
    break;

  case 105: /* rel_attr: LBRACE value value_list RBRACE  */
#line 1075 "yacc_sql.y"
                                                  {
      (yyval.rel_attr) = new RelAttrSqlNode;
      if ((yyvsp[-1].value_list) == nullptr) {
        (yyvsp[-1].value_list) = new std::vector<Value>;
      }
      (yyvsp[-1].value_list)->push_back(*(yyvsp[-2].value));
      delete (yyvsp[-2].value);
      (yyval.rel_attr)->expression = new ListQueryExpr(*(yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);
    }
#line 3018 "yacc_sql.cpp"
    break;

  case 106: /* sub_query: LBRACE SELECT rel_attr alias FROM ID alias rel_condition_list where group order having RBRACE  */
#line 1089 "yacc_sql.y"
    {
      SelectSqlNode *sub_query = new SelectSqlNode;
      (yyvsp[-10].rel_attr)->alias.reset((yyvsp[-9].string));
      sub_query->attributes.push_back(*(yyvsp[-10].rel_attr));
      if ((yyvsp[-5].relationAndConditionTempList) != nullptr) {
        sub_query->relations.swap((yyvsp[-5].relationAndConditionTempList)->_rel_list);
        delete (yyvsp[-5].relationAndConditionTempList);
      }
      RelationSqlNode node;
      init_relation_sql_node((yyvsp[-7].string), (yyvsp[-6].string), node);
      sub_query->relations.push_back(node);
      std::reverse(sub_query->relations.begin(), sub_query->relations.end());
      if ((yyvsp[-4].condition_list) != nullptr) {
        sub_query->conditions.swap(*(yyvsp[-4].condition_list));
        delete (yyvsp[-4].condition_list);
      }
      if ((yyvsp[-3].rel_attr_list) != nullptr) {
        std::reverse((yyvsp[-3].rel_attr_list)->begin(), (yyvsp[-3].rel_attr_list)->end());
        sub_query->groups.swap(*(yyvsp[-3].rel_attr_list));
        delete (yyvsp[-3].rel_attr_list);
      }
      if ((yyvsp[-2].order_condition_list) != nullptr) {
        std::reverse((yyvsp[-2].order_condition_list)->begin(), (yyvsp[-2].order_condition_list)->end());
        sub_query->orders.swap(*(yyvsp[-2].order_condition_list));
        delete (yyvsp[-2].order_condition_list);
      }
      if ((yyvsp[-1].condition_list) != nullptr) {
        std::reverse((yyvsp[-1].condition_list)->begin(), (yyvsp[-1].condition_list)->end());
        sub_query->havings.swap(*(yyvsp[-1].condition_list));
        delete (yyvsp[-1].condition_list);
      }
      (yyval.sub_select) = sub_query;
      delete (yyvsp[-10].rel_attr);
      free((yyvsp[-7].string));
    }
#line 3058 "yacc_sql.cpp"
    break;

  case 107: /* attr_list: %empty  */
#line 1127 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 3066 "yacc_sql.cpp"
    break;

  case 108: /* attr_list: COMMA rel_attr alias attr_list  */
#line 1130 "yacc_sql.y"
                                     {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      } else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      (yyvsp[-2].rel_attr)->alias.reset((yyvsp[-1].string));
      (yyval.rel_attr_list)->emplace_back(*(yyvsp[-2].rel_attr));
      delete (yyvsp[-2].rel_attr);
    }
#line 3081 "yacc_sql.cpp"
    break;

  case 109: /* attr_list: COMMA ID DOT '*' attr_list  */
#line 1140 "yacc_sql.y"
                                 {
      if ((yyvsp[0].rel_attr_list) != nullptr) {
        (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      }else {
        (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      }
      RelAttrSqlNode node;
      node.relation_name = (yyvsp[-3].string);
      node.attribute_name = "*";
      (yyval.rel_attr_list)->push_back(node);
      free((yyvsp[-3].string));
    }
#line 3098 "yacc_sql.cpp"
    break;

  case 110: /* rel_list: %empty  */
#line 1156 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 3106 "yacc_sql.cpp"
    break;

  case 111: /* rel_list: COMMA ID rel_list  */
#line 1159 "yacc_sql.y"
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 3121 "yacc_sql.cpp"
    break;

  case 112: /* rel_condition_list: %empty  */
#line 1173 "yacc_sql.y"
    {
      (yyval.relationAndConditionTempList) = nullptr;
    }
#line 3129 "yacc_sql.cpp"
    break;

  case 113: /* rel_condition_list: COMMA ID alias rel_condition_list  */
#line 1177 "yacc_sql.y"
                                        {
      if ((yyvsp[0].relationAndConditionTempList) != nullptr) {
        (yyval.relationAndConditionTempList) = (yyvsp[0].relationAndConditionTempList);
      } else {
        (yyval.relationAndConditionTempList) = new RelationAndConditionTempList;
      }
      RelationSqlNode node;
      init_relation_sql_node((yyvsp[-2].string), (yyvsp[-1].string), node);
      (yyval.relationAndConditionTempList)->_rel_list.push_back(node);
      free((yyvsp[-2].string));
    }
#line 3145 "yacc_sql.cpp"
    break;

  case 114: /* rel_condition_list: INNER JOIN ID alias condition_list rel_condition_list  */
#line 1189 "yacc_sql.y"
                                                           {
      if ((yyvsp[0].relationAndConditionTempList) != nullptr) {
        (yyval.relationAndConditionTempList) = (yyvsp[0].relationAndConditionTempList);
      } else {
        (yyval.relationAndConditionTempList) = new RelationAndConditionTempList;
      }
      if((yyvsp[-1].condition_list) != nullptr) {
          (yyval.relationAndConditionTempList)->_condition_list.push_back(*((yyvsp[-1].condition_list)));
          delete (yyvsp[-1].condition_list);
      }
      RelationSqlNode node;
      init_relation_sql_node((yyvsp[-3].string), (yyvsp[-2].string), node);
      (yyval.relationAndConditionTempList)->_rel_list.push_back(node);
      free((yyvsp[-3].string));
      std::cout << "inner join parse finished" << std::endl;
    }
#line 3166 "yacc_sql.cpp"
    break;

  case 115: /* where: %empty  */
#line 1209 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3174 "yacc_sql.cpp"
    break;

  case 116: /* where: WHERE condition_list  */
#line 1212 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 3182 "yacc_sql.cpp"
    break;

  case 117: /* having: %empty  */
#line 1217 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3190 "yacc_sql.cpp"
    break;

  case 118: /* having: HAVING condition_list  */
#line 1220 "yacc_sql.y"
                            {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3198 "yacc_sql.cpp"
    break;

  case 119: /* condition_list: %empty  */
#line 1226 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3206 "yacc_sql.cpp"
    break;

  case 120: /* condition_list: condition  */
#line 1229 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyvsp[0].condition)->next_or_link = 0;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 3217 "yacc_sql.cpp"
    break;

  case 121: /* condition_list: condition AND condition_list  */
#line 1235 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->next_or_link = 0;
      std::cout << "enter and" << std::endl;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 3229 "yacc_sql.cpp"
    break;

  case 122: /* condition_list: condition OR condition_list  */
#line 1242 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->next_or_link = 1;
      std::cout << "enter or" << std::endl;
      std::cout << (yyvsp[-2].condition)->next_or_link << std::endl;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      std::cout << (yyval.condition_list)->back().next_or_link << std::endl;
      delete (yyvsp[-2].condition);
    }
#line 3243 "yacc_sql.cpp"
    break;

  case 123: /* condition_list: ON condition_list  */
#line 1251 "yacc_sql.y"
                        {
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.condition_list) = (yyvsp[0].condition_list);
      }else {
        (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      }
    }
#line 3255 "yacc_sql.cpp"
    break;

  case 124: /* condition: rel_attr comp_op rel_attr  */
#line 1261 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      if ((yyvsp[-2].rel_attr)->expression == nullptr) {
        (yyval.condition)->left_is_attr = 1;
        (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      }else if ((yyvsp[-2].rel_attr)->expression->type() != ExprType::VALUE) {
        (yyval.condition)->left_is_attr = 1;
        (yyval.condition)->left_attr = *(yyvsp[-2].rel_attr);
      }else {
        (yyval.condition)->left_is_attr = 0;
        (yyvsp[-2].rel_attr)->expression->try_get_value((yyval.condition)->left_value);
      }
      
      if ((yyvsp[0].rel_attr)->expression == nullptr) {
        (yyval.condition)->right_is_attr = 1;
        (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      }else if ((yyvsp[0].rel_attr)->expression->type() != ExprType::VALUE) {
        (yyval.condition)->right_is_attr = 1;
        (yyval.condition)->right_attr = *(yyvsp[0].rel_attr);
      }else {
        (yyval.condition)->right_is_attr = 0;
        (yyvsp[0].rel_attr)->expression->try_get_value((yyval.condition)->right_value);
      }
      (yyval.condition)->comp = (yyvsp[-1].comp);
      std::cout << "comp type: " << (yyval.condition)->comp << std::endl;
      delete (yyvsp[-2].rel_attr);
      delete (yyvsp[0].rel_attr);
    }
#line 3288 "yacc_sql.cpp"
    break;

  case 125: /* group: %empty  */
#line 1293 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 3296 "yacc_sql.cpp"
    break;

  case 126: /* group: GROUP BY group_item group_list  */
#line 1297 "yacc_sql.y"
  {
    if ((yyvsp[0].rel_attr_list) == nullptr) {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }else {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
    (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
    delete (yyvsp[-1].rel_attr);
  }
#line 3310 "yacc_sql.cpp"
    break;

  case 127: /* group_list: %empty  */
#line 1308 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 3318 "yacc_sql.cpp"
    break;

  case 128: /* group_list: COMMA group_item group_list  */
#line 1311 "yacc_sql.y"
                                {
    if ((yyvsp[0].rel_attr_list) == nullptr) {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }else {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
    (yyval.rel_attr_list)->push_back(*(yyvsp[-1].rel_attr));
    delete (yyvsp[-1].rel_attr);
  }
#line 3332 "yacc_sql.cpp"
    break;

  case 129: /* group_item: ID  */
#line 1322 "yacc_sql.y"
     {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
    free((yyvsp[0].string));
  }
#line 3342 "yacc_sql.cpp"
    break;

  case 130: /* group_item: ID DOT ID  */
#line 1327 "yacc_sql.y"
              {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->relation_name = (yyvsp[-2].string);
    (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
    free((yyvsp[-2].string));
    free((yyvsp[0].string));
  }
#line 3354 "yacc_sql.cpp"
    break;

  case 131: /* order: %empty  */
#line 1337 "yacc_sql.y"
  {
    (yyval.order_condition_list) = nullptr;
  }
#line 3362 "yacc_sql.cpp"
    break;

  case 132: /* order: ORDER BY order_condition order_condition_list  */
#line 1340 "yacc_sql.y"
                                                  {
    if ((yyvsp[0].order_condition_list) != nullptr) {
      (yyval.order_condition_list) = (yyvsp[0].order_condition_list);
    }else {
      (yyval.order_condition_list) = new std::vector<OrderSqlNode>;
    }
    (yyval.order_condition_list)->emplace_back(*(yyvsp[-1].order_condition));
    delete (yyvsp[-1].order_condition);
  }
#line 3376 "yacc_sql.cpp"
    break;

  case 133: /* order_condition_list: %empty  */
#line 1351 "yacc_sql.y"
  {
    (yyval.order_condition_list) = nullptr;
  }
#line 3384 "yacc_sql.cpp"
    break;

  case 134: /* order_condition_list: COMMA order_condition order_condition_list  */
#line 1354 "yacc_sql.y"
                                               {
    if ((yyvsp[0].order_condition_list) != nullptr) {
      (yyval.order_condition_list) = (yyvsp[0].order_condition_list);
    }else {
      (yyval.order_condition_list) = new std::vector<OrderSqlNode>;
    }
    (yyval.order_condition_list)->emplace_back(*(yyvsp[-1].order_condition));
    delete (yyvsp[-1].order_condition);
  }
#line 3398 "yacc_sql.cpp"
    break;

  case 135: /* order_condition: ID  */
#line 1366 "yacc_sql.y"
     {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[0].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[0].string));
  }
#line 3409 "yacc_sql.cpp"
    break;

  case 136: /* order_condition: ID DOT ID  */
#line 1372 "yacc_sql.y"
              {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.relation_name = (yyvsp[-2].string);
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[0].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[-2].string));
    free((yyvsp[0].string));
  }
#line 3422 "yacc_sql.cpp"
    break;

  case 137: /* order_condition: ID ASC  */
#line 1380 "yacc_sql.y"
           {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[-1].string));
  }
#line 3433 "yacc_sql.cpp"
    break;

  case 138: /* order_condition: ID DESC  */
#line 1386 "yacc_sql.y"
            {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 0;
    free((yyvsp[-1].string));
  }
#line 3444 "yacc_sql.cpp"
    break;

  case 139: /* order_condition: ID DOT ID ASC  */
#line 1392 "yacc_sql.y"
                  {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.relation_name = (yyvsp[-3].string);
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 3457 "yacc_sql.cpp"
    break;

  case 140: /* order_condition: ID DOT ID DESC  */
#line 1400 "yacc_sql.y"
                   {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.relation_name = (yyvsp[-3].string);
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 0;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 3470 "yacc_sql.cpp"
    break;

  case 141: /* agg_func: MAX  */
#line 1409 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_MAX; }
#line 3476 "yacc_sql.cpp"
    break;

  case 142: /* agg_func: MIN  */
#line 1410 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_MIN; }
#line 3482 "yacc_sql.cpp"
    break;

  case 143: /* agg_func: COUNT  */
#line 1411 "yacc_sql.y"
            { (yyval.agg_func) = FUNC_COUNT; }
#line 3488 "yacc_sql.cpp"
    break;

  case 144: /* agg_func: AVG  */
#line 1412 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_AVG; }
#line 3494 "yacc_sql.cpp"
    break;

  case 145: /* agg_func: SUM  */
#line 1413 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_SUM; }
#line 3500 "yacc_sql.cpp"
    break;

  case 146: /* func: LENGTH  */
#line 1416 "yacc_sql.y"
             { (yyval.func) = FUNC_LENGTH; }
#line 3506 "yacc_sql.cpp"
    break;

  case 147: /* func: ROUND  */
#line 1417 "yacc_sql.y"
            { (yyval.func) = FUNC_ROUND; }
#line 3512 "yacc_sql.cpp"
    break;

  case 148: /* func: DATE_FORMAT  */
#line 1418 "yacc_sql.y"
                  { (yyval.func) = FUNC_DATE_FORMAT; }
#line 3518 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: EQ  */
#line 1421 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3524 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: LT  */
#line 1422 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3530 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: GT  */
#line 1423 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3536 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: LE  */
#line 1424 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3542 "yacc_sql.cpp"
    break;

  case 153: /* comp_op: GE  */
#line 1425 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3548 "yacc_sql.cpp"
    break;

  case 154: /* comp_op: NE  */
#line 1426 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3554 "yacc_sql.cpp"
    break;

  case 155: /* comp_op: IS  */
#line 1427 "yacc_sql.y"
         { (yyval.comp) = IS_NULL; }
#line 3560 "yacc_sql.cpp"
    break;

  case 156: /* comp_op: IS NOT  */
#line 1428 "yacc_sql.y"
             { (yyval.comp) = NOT_NULL; }
#line 3566 "yacc_sql.cpp"
    break;

  case 157: /* comp_op: EXISTS  */
#line 1429 "yacc_sql.y"
             { (yyval.comp) = EXISTS_OP; }
#line 3572 "yacc_sql.cpp"
    break;

  case 158: /* comp_op: NOT EXISTS  */
#line 1430 "yacc_sql.y"
                 { (yyval.comp) = NOT_EXISTS_OP; }
#line 3578 "yacc_sql.cpp"
    break;

  case 159: /* comp_op: IN  */
#line 1431 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 3584 "yacc_sql.cpp"
    break;

  case 160: /* comp_op: NOT IN  */
#line 1432 "yacc_sql.y"
             { (yyval.comp) = NOT_IN_OP; }
#line 3590 "yacc_sql.cpp"
    break;

  case 161: /* comp_op: LIKE  */
#line 1433 "yacc_sql.y"
           { (yyval.comp) = LIKE_OP; }
#line 3596 "yacc_sql.cpp"
    break;

  case 162: /* comp_op: NOT LIKE  */
#line 1434 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE_OP; }
#line 3602 "yacc_sql.cpp"
    break;

  case 163: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1439 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3616 "yacc_sql.cpp"
    break;

  case 164: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1452 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3625 "yacc_sql.cpp"
    break;

  case 165: /* set_variable_stmt: SET ID EQ value  */
#line 1460 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3637 "yacc_sql.cpp"
    break;

  case 166: /* set_variable_stmt: SET ID EQ '-' value  */
#line 1468 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name = (yyvsp[-3].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-3].string));
      delete (yyvsp[0].value);
    }
#line 3649 "yacc_sql.cpp"
    break;


#line 3653 "yacc_sql.cpp"

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
  *++yylsp = yyloc;

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
          = {yyssp, yytoken, &yylloc};
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
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
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

#line 1480 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}

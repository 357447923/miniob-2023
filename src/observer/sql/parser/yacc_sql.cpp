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
  YYSYMBOL_UNIQUE = 9,                     /* UNIQUE  */
  YYSYMBOL_VIEW = 10,                      /* VIEW  */
  YYSYMBOL_CALC = 11,                      /* CALC  */
  YYSYMBOL_SELECT = 12,                    /* SELECT  */
  YYSYMBOL_SHOW = 13,                      /* SHOW  */
  YYSYMBOL_SYNC = 14,                      /* SYNC  */
  YYSYMBOL_INSERT = 15,                    /* INSERT  */
  YYSYMBOL_DELETE = 16,                    /* DELETE  */
  YYSYMBOL_UPDATE = 17,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 18,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 19,                    /* RBRACE  */
  YYSYMBOL_COMMA = 20,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 21,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 22,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 23,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 24,                     /* INT_T  */
  YYSYMBOL_STRING_T = 25,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 26,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 27,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 28,                    /* TEXT_T  */
  YYSYMBOL_null = 29,                      /* null  */
  YYSYMBOL_IS = 30,                        /* IS  */
  YYSYMBOL_NOT = 31,                       /* NOT  */
  YYSYMBOL_HELP = 32,                      /* HELP  */
  YYSYMBOL_EXIT = 33,                      /* EXIT  */
  YYSYMBOL_DOT = 34,                       /* DOT  */
  YYSYMBOL_INTO = 35,                      /* INTO  */
  YYSYMBOL_VALUES = 36,                    /* VALUES  */
  YYSYMBOL_FROM = 37,                      /* FROM  */
  YYSYMBOL_WHERE = 38,                     /* WHERE  */
  YYSYMBOL_HAVING = 39,                    /* HAVING  */
  YYSYMBOL_AND = 40,                       /* AND  */
  YYSYMBOL_OR = 41,                        /* OR  */
  YYSYMBOL_SET = 42,                       /* SET  */
  YYSYMBOL_INNER = 43,                     /* INNER  */
  YYSYMBOL_JOIN = 44,                      /* JOIN  */
  YYSYMBOL_ON = 45,                        /* ON  */
  YYSYMBOL_GROUP = 46,                     /* GROUP  */
  YYSYMBOL_ORDER = 47,                     /* ORDER  */
  YYSYMBOL_BY = 48,                        /* BY  */
  YYSYMBOL_ASC = 49,                       /* ASC  */
  YYSYMBOL_DESC = 50,                      /* DESC  */
  YYSYMBOL_LOAD = 51,                      /* LOAD  */
  YYSYMBOL_DATA = 52,                      /* DATA  */
  YYSYMBOL_INFILE = 53,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 54,                   /* EXPLAIN  */
  YYSYMBOL_MAX = 55,                       /* MAX  */
  YYSYMBOL_MIN = 56,                       /* MIN  */
  YYSYMBOL_AVG = 57,                       /* AVG  */
  YYSYMBOL_COUNT = 58,                     /* COUNT  */
  YYSYMBOL_SUM = 59,                       /* SUM  */
  YYSYMBOL_LENGTH = 60,                    /* LENGTH  */
  YYSYMBOL_ROUND = 61,                     /* ROUND  */
  YYSYMBOL_DATE_FORMAT = 62,               /* DATE_FORMAT  */
  YYSYMBOL_EXISTS = 63,                    /* EXISTS  */
  YYSYMBOL_IN = 64,                        /* IN  */
  YYSYMBOL_EQ = 65,                        /* EQ  */
  YYSYMBOL_LT = 66,                        /* LT  */
  YYSYMBOL_GT = 67,                        /* GT  */
  YYSYMBOL_LE = 68,                        /* LE  */
  YYSYMBOL_GE = 69,                        /* GE  */
  YYSYMBOL_NE = 70,                        /* NE  */
  YYSYMBOL_LIKE = 71,                      /* LIKE  */
  YYSYMBOL_AS = 72,                        /* AS  */
  YYSYMBOL_NUMBER = 73,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 74,                     /* FLOAT  */
  YYSYMBOL_ID = 75,                        /* ID  */
  YYSYMBOL_SSS = 76,                       /* SSS  */
  YYSYMBOL_77_ = 77,                       /* '+'  */
  YYSYMBOL_78_ = 78,                       /* '-'  */
  YYSYMBOL_79_ = 79,                       /* '*'  */
  YYSYMBOL_80_ = 80,                       /* '/'  */
  YYSYMBOL_UMINUS = 81,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 82,                  /* $accept  */
  YYSYMBOL_commands = 83,                  /* commands  */
  YYSYMBOL_command_wrapper = 84,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 85,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 86,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 87,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 88,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 89,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 90,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 91,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 92,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 93,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 94,         /* create_index_stmt  */
  YYSYMBOL_idx_attr_list = 95,             /* idx_attr_list  */
  YYSYMBOL_drop_index_stmt = 96,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 97,         /* create_table_stmt  */
  YYSYMBOL_id_list = 98,                   /* id_list  */
  YYSYMBOL_create_table_select = 99,       /* create_table_select  */
  YYSYMBOL_attr_def_list = 100,            /* attr_def_list  */
  YYSYMBOL_attr_def = 101,                 /* attr_def  */
  YYSYMBOL_number = 102,                   /* number  */
  YYSYMBOL_type = 103,                     /* type  */
  YYSYMBOL_insert_stmt = 104,              /* insert_stmt  */
  YYSYMBOL_value_lists = 105,              /* value_lists  */
  YYSYMBOL_value_list = 106,               /* value_list  */
  YYSYMBOL_value = 107,                    /* value  */
  YYSYMBOL_delete_stmt = 108,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 109,              /* update_stmt  */
  YYSYMBOL_set_clause_list = 110,          /* set_clause_list  */
  YYSYMBOL_set_clause = 111,               /* set_clause  */
  YYSYMBOL_select_stmt = 112,              /* select_stmt  */
  YYSYMBOL_calc_stmt = 113,                /* calc_stmt  */
  YYSYMBOL_simple_func_item = 114,         /* simple_func_item  */
  YYSYMBOL_simple_func_list = 115,         /* simple_func_list  */
  YYSYMBOL_expression_list = 116,          /* expression_list  */
  YYSYMBOL_expression = 117,               /* expression  */
  YYSYMBOL_select_attr = 118,              /* select_attr  */
  YYSYMBOL_alias = 119,                    /* alias  */
  YYSYMBOL_rel_attr = 120,                 /* rel_attr  */
  YYSYMBOL_sub_query = 121,                /* sub_query  */
  YYSYMBOL_attr_list = 122,                /* attr_list  */
  YYSYMBOL_rel_list = 123,                 /* rel_list  */
  YYSYMBOL_rel_condition_list = 124,       /* rel_condition_list  */
  YYSYMBOL_where = 125,                    /* where  */
  YYSYMBOL_having = 126,                   /* having  */
  YYSYMBOL_condition_list = 127,           /* condition_list  */
  YYSYMBOL_condition = 128,                /* condition  */
  YYSYMBOL_group = 129,                    /* group  */
  YYSYMBOL_group_list = 130,               /* group_list  */
  YYSYMBOL_group_item = 131,               /* group_item  */
  YYSYMBOL_order = 132,                    /* order  */
  YYSYMBOL_order_condition_list = 133,     /* order_condition_list  */
  YYSYMBOL_order_condition = 134,          /* order_condition  */
  YYSYMBOL_agg_func = 135,                 /* agg_func  */
  YYSYMBOL_func = 136,                     /* func  */
  YYSYMBOL_comp_op = 137,                  /* comp_op  */
  YYSYMBOL_load_data_stmt = 138,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 139,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 140,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 141             /* opt_semicolon  */
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
#define YYFINAL  84
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   420

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  172
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  346

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   332


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
       2,     2,    79,    77,     2,    78,     2,    80,     2,     2,
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
      75,    76,    81
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   250,   250,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   281,   287,   292,   298,   304,   310,   316,
     323,   329,   337,   354,   375,   378,   390,   400,   418,   425,
     441,   449,   478,   481,   493,   506,   522,   525,   538,   547,
     560,   564,   570,   573,   574,   575,   576,   577,   580,   603,
     629,   632,   649,   671,   674,   683,   695,   699,   703,   708,
     716,   728,   744,   754,   762,   770,   779,   789,   828,   845,
     855,   880,   911,   914,   927,   932,   943,   946,   949,   952,
     955,   959,   962,   967,   972,   978,   984,   991,   996,  1001,
    1019,  1042,  1049,  1059,  1074,  1077,  1080,  1086,  1121,  1126,
    1139,  1178,  1181,  1191,  1207,  1210,  1224,  1228,  1240,  1260,
    1263,  1268,  1271,  1277,  1280,  1286,  1293,  1302,  1311,  1344,
    1347,  1359,  1362,  1373,  1378,  1388,  1391,  1402,  1405,  1417,
    1423,  1431,  1437,  1443,  1451,  1460,  1461,  1462,  1463,  1464,
    1467,  1468,  1469,  1472,  1473,  1474,  1475,  1476,  1477,  1478,
    1479,  1480,  1481,  1482,  1483,  1484,  1485,  1489,  1502,  1510,
    1518,  1528,  1529
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
  "DROP", "TABLE", "TABLES", "INDEX", "UNIQUE", "VIEW", "CALC", "SELECT",
  "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T",
  "FLOAT_T", "DATE_T", "TEXT_T", "null", "IS", "NOT", "HELP", "EXIT",
  "DOT", "INTO", "VALUES", "FROM", "WHERE", "HAVING", "AND", "OR", "SET",
  "INNER", "JOIN", "ON", "GROUP", "ORDER", "BY", "ASC", "DESC", "LOAD",
  "DATA", "INFILE", "EXPLAIN", "MAX", "MIN", "AVG", "COUNT", "SUM",
  "LENGTH", "ROUND", "DATE_FORMAT", "EXISTS", "IN", "EQ", "LT", "GT", "LE",
  "GE", "NE", "LIKE", "AS", "NUMBER", "FLOAT", "ID", "SSS", "'+'", "'-'",
  "'*'", "'/'", "UMINUS", "$accept", "commands", "command_wrapper",
  "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt",
  "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "create_index_stmt", "idx_attr_list",
  "drop_index_stmt", "create_table_stmt", "id_list", "create_table_select",
  "attr_def_list", "attr_def", "number", "type", "insert_stmt",
  "value_lists", "value_list", "value", "delete_stmt", "update_stmt",
  "set_clause_list", "set_clause", "select_stmt", "calc_stmt",
  "simple_func_item", "simple_func_list", "expression_list", "expression",
  "select_attr", "alias", "rel_attr", "sub_query", "attr_list", "rel_list",
  "rel_condition_list", "where", "having", "condition_list", "condition",
  "group", "group_list", "group_item", "order", "order_condition_list",
  "order_condition", "agg_func", "func", "comp_op", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-265)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-64)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     349,   178,    55,   215,   143,    42,  -265,    20,   -21,    -4,
    -265,  -265,  -265,  -265,  -265,     8,    27,    26,   349,    59,
      82,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,
    -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,
    -265,    32,    44,    79,    70,    74,    76,   215,  -265,  -265,
    -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,    60,
    -265,   215,  -265,  -265,    62,   -39,   215,   134,    83,  -265,
     -19,    47,   119,   -19,  -265,   215,  -265,    93,    95,   124,
     112,  -265,   126,  -265,  -265,  -265,  -265,     0,   136,   108,
       3,  -265,   152,    12,   131,  -265,   215,   215,   215,   215,
     215,  -265,     9,   196,   201,   -10,   239,    10,   -52,   153,
    -265,   206,   155,   212,    85,   199,   198,   162,   -16,   163,
     166,   226,  -265,  -265,   167,   207,   176,  -265,   179,  -265,
    -265,  -265,    56,    56,  -265,  -265,  -265,   180,  -265,  -265,
    -265,   215,   -19,    81,   237,   212,  -265,    53,  -265,   -19,
     274,  -265,  -265,   211,   240,   205,  -265,   194,   198,   249,
     211,  -265,   247,   221,     4,  -265,   268,   227,   284,  -265,
     288,    19,   271,   211,   289,  -265,  -265,   -19,   211,     2,
     276,   -19,   292,    91,   205,   275,  -265,   103,   100,  -265,
     162,  -265,   310,  -265,  -265,  -265,  -265,  -265,   300,   166,
    -265,   290,   301,   246,   304,   248,   305,  -265,  -265,   250,
     289,  -265,   206,    85,   251,   283,   198,   -27,   212,  -265,
     211,   289,  -265,   297,   -17,  -265,  -265,  -265,  -265,  -265,
    -265,  -265,  -265,  -265,   239,   205,   205,   325,   211,  -265,
    -265,  -265,   280,   278,     4,  -265,     5,   336,   282,   284,
       5,   -19,  -265,  -265,   -19,   293,   312,   212,  -265,   289,
     340,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,  -265,
     348,  -265,  -265,   294,   354,   336,  -265,  -265,     2,     2,
     -19,   326,   328,  -265,   357,   358,  -265,   359,  -265,   361,
     198,  -265,   205,   302,   335,   345,   358,   367,  -265,   311,
    -265,  -265,   312,     2,   353,   368,   314,   205,  -265,  -265,
     151,   359,   328,  -265,   315,   302,  -265,    -8,   372,  -265,
     211,   289,  -265,   345,  -265,   368,   318,  -265,  -265,   314,
    -265,   289,   375,   376,  -265,   164,   372,   377,   358,  -265,
    -265,  -265,  -265,   358,  -265,  -265
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
      26,    27,    28,    24,    23,     0,     0,     0,     0,     0,
     171,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,    69,   145,
     146,   148,   147,   149,   150,   151,   152,    66,    67,    93,
      68,     0,    92,    79,    84,     0,     0,     0,    93,   101,
     104,   107,     0,   104,   108,     0,    30,     0,     0,     0,
       0,    31,     0,   168,     1,   172,     2,     0,     0,     0,
       0,    29,     0,     0,     0,    91,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,    92,     0,     0,
     105,    82,     0,   111,    92,     0,   119,     0,     0,     0,
       0,     0,    38,    44,     0,     0,     0,    40,     0,    90,
      94,    85,    86,    87,    88,    89,    95,     0,    97,    98,
      99,     0,   104,     0,     0,   111,   106,     0,    78,   104,
       0,   102,    80,     0,     0,   123,    70,     0,   119,    73,
       0,   169,     0,     0,    46,    45,     0,     0,    42,    36,
       0,     0,     0,     0,    63,   109,   103,   104,     0,   116,
      93,   104,     0,     0,   123,     0,   120,   124,     0,    71,
       0,   170,     0,    53,    54,    55,    56,    57,    49,     0,
      51,     0,     0,     0,     0,     0,     0,    96,   100,     0,
      63,    64,    82,     0,     0,     0,   119,     0,   111,    81,
       0,    63,   127,   159,     0,   161,   163,   153,   154,   155,
     156,   157,   158,   165,     0,   123,   123,     0,     0,    74,
      76,    72,     0,     0,    46,    50,    37,    34,     0,    42,
       0,   104,    65,    83,   104,     0,   129,   111,   112,    63,
       0,   160,   162,   164,   166,   128,   125,   126,    75,   167,
       0,    47,    39,     0,     0,    34,    43,    41,   116,   116,
     104,     0,   135,   113,     0,    60,    48,   114,    32,     0,
     119,   117,   123,     0,     0,   121,    60,     0,    58,     0,
      35,    33,   129,   116,   133,   131,     0,   123,    77,    59,
       0,   114,   135,   118,     0,     0,   130,   139,   137,   122,
       0,    63,   115,   121,   134,   131,     0,   141,   142,     0,
     136,    63,     0,     0,   132,   140,   137,     0,    60,   110,
     143,   144,   138,    60,    61,    62
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -265,  -265,   379,  -265,  -265,  -265,  -265,  -265,  -265,  -265,
    -265,  -265,  -265,   123,  -265,  -265,   156,   -88,   157,   203,
     161,  -265,  -265,  -264,  -171,   -67,  -265,  -265,   216,  -265,
       7,  -265,   260,   197,   316,    34,  -265,   -69,    -3,   220,
    -134,    99,  -259,  -152,    88,  -170,  -265,   111,    89,   101,
     105,    84,    86,  -265,     1,  -265,  -265,  -265,  -265,  -265
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   274,    31,    32,   206,   122,   202,   164,
     104,   198,    33,   298,   144,    62,    34,    35,   158,   159,
     123,    37,    70,   148,    63,    71,    72,   111,   185,    74,
     151,   300,   216,   156,   308,   186,   187,   282,   316,   305,
     295,   330,   318,    65,    66,   234,    38,    39,    40,    86
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     107,    73,   127,   211,   113,    75,   189,    36,   114,   140,
     141,   176,     4,    48,   222,     4,    78,     4,   120,   290,
     291,   126,   214,   130,   199,    36,   326,   145,   136,   -63,
     143,   129,   309,   200,   101,   201,   102,    64,   208,   252,
     103,   327,   328,   137,   313,   215,   262,   263,   130,    76,
     260,   161,   257,   109,   264,    77,   110,    57,    58,    84,
      60,    45,   160,    46,   256,   266,   267,    97,    98,    99,
     100,    79,   121,   172,   344,   121,   174,   121,    82,   345,
     179,    93,    96,    80,   258,    85,   182,    89,   284,    97,
      98,    99,   100,   191,    94,    95,    97,    98,    99,   100,
     105,    93,    81,   142,   152,   153,   210,    87,   212,   105,
      48,   213,   218,    54,    55,    56,   221,   108,   237,    88,
      48,   239,   303,   283,    97,    98,    99,   100,   165,    48,
      64,   132,   133,   134,   135,    99,   100,   319,   302,    97,
      98,    99,   100,   235,   236,    90,   106,   181,   178,    91,
     332,    92,    47,   259,    57,    58,   112,    60,   272,   173,
     337,    67,   277,    48,    57,    58,   117,    60,   115,   220,
     116,   268,    48,    57,    58,   171,    60,   118,   238,   119,
      48,   124,   278,   125,    41,   279,    42,    43,    44,    49,
      50,    51,    52,    53,    54,    55,    56,   128,    49,    50,
      51,    52,    53,    54,    55,    56,   130,    57,    58,    59,
      60,   292,    61,   340,   341,   138,    57,    58,    68,    60,
     139,    61,    69,    67,    57,    58,   147,    60,   146,   320,
     149,   265,   150,    47,    48,   154,   155,   157,     4,   162,
      48,   163,   166,   321,    48,   193,   194,   195,   196,   197,
     184,   168,   167,   331,   169,   170,   175,    67,   183,   188,
      49,    50,    51,    52,    53,    54,    55,    56,    48,   190,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   192,    61,    57,    58,   203,    60,    57,    58,
      59,    60,    67,    61,    49,    50,    51,    52,    53,    54,
      55,    56,   204,    48,   205,   223,   224,   207,   209,   143,
     217,   219,    57,    58,    59,    60,   242,    61,   243,   245,
     246,   247,   248,   249,   250,   251,   254,   255,   261,    49,
      50,    51,    52,    53,    54,    55,    56,   106,   225,   226,
     227,   228,   229,   230,   231,   232,   233,    57,    58,   180,
      60,   101,    61,     1,     2,   269,   273,   275,   281,   285,
       3,     4,     5,     6,     7,     8,     9,   286,   280,   287,
      10,    11,    12,   288,   293,   294,   296,   304,   297,   299,
     301,    13,    14,   306,   307,   310,   311,   314,   315,   317,
     324,    15,   329,   335,   338,   339,   343,    83,   289,    16,
      17,   271,   244,    18,   270,   276,   241,   177,   240,   253,
     322,   333,   131,   312,   334,   336,   325,   323,     0,     0,
     342
};

static const yytype_int16 yycheck[] =
{
      67,     4,    90,   174,    73,     4,   158,     0,    75,    19,
      20,   145,    12,    29,   184,    12,    37,    12,    18,   278,
     279,    18,    20,    75,    20,    18,    34,    79,    19,    19,
      20,    19,   296,    29,    73,    31,    75,     3,    19,   210,
      79,    49,    50,    34,   303,    43,    63,    64,    75,     7,
     221,   118,    79,    72,    71,    35,    75,    73,    74,     0,
      76,     6,    78,     8,   216,   235,   236,    77,    78,    79,
      80,    75,    72,   142,   338,    72,   143,    72,    52,   343,
     149,    47,    20,    75,   218,     3,   153,     8,   259,    77,
      78,    79,    80,   160,    34,    61,    77,    78,    79,    80,
      66,    67,    75,   106,    19,    20,   173,    75,   177,    75,
      29,   178,   181,    60,    61,    62,   183,    34,    18,    75,
      29,   188,   292,   257,    77,    78,    79,    80,   121,    29,
      96,    97,    98,    99,   100,    79,    80,   307,   290,    77,
      78,    79,    80,    40,    41,    75,    12,   150,   147,    75,
     321,    75,    18,   220,    73,    74,    37,    76,   246,    78,
     331,    18,   250,    29,    73,    74,    42,    76,    75,    78,
      75,   238,    29,    73,    74,   141,    76,    65,    78,    53,
      29,    45,   251,    75,     6,   254,     8,     9,    10,    55,
      56,    57,    58,    59,    60,    61,    62,    45,    55,    56,
      57,    58,    59,    60,    61,    62,    75,    73,    74,    75,
      76,   280,    78,    49,    50,    19,    73,    74,    75,    76,
      19,    78,    79,    18,    73,    74,    20,    76,    75,    78,
      75,   234,    20,    18,    29,    36,    38,    75,    12,    76,
      29,    75,    75,   310,    29,    24,    25,    26,    27,    28,
      45,    75,    45,   320,    75,    75,    19,    18,    18,    65,
      55,    56,    57,    58,    59,    60,    61,    62,    29,    20,
      55,    56,    57,    58,    59,    60,    61,    62,    73,    74,
      75,    76,    35,    78,    73,    74,    18,    76,    73,    74,
      75,    76,    18,    78,    55,    56,    57,    58,    59,    60,
      61,    62,    75,    29,    20,    30,    31,    19,    37,    20,
      34,    19,    73,    74,    75,    76,     6,    78,    18,    29,
      19,    75,    18,    75,    19,    75,    75,    44,    31,    55,
      56,    57,    58,    59,    60,    61,    62,    12,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    73,    74,    75,
      76,    73,    78,     4,     5,    75,    20,    75,    46,    19,
      11,    12,    13,    14,    15,    16,    17,    19,    75,    75,
      21,    22,    23,    19,    48,    47,    19,    75,    20,    20,
      19,    32,    33,    48,    39,    18,    75,    34,    20,    75,
      75,    42,    20,    75,    19,    19,    19,    18,   275,    50,
      51,   244,   199,    54,   243,   249,   190,   147,   188,   212,
     311,   323,    96,   302,   325,   329,   315,   312,    -1,    -1,
     336
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,    11,    12,    13,    14,    15,    16,    17,
      21,    22,    23,    32,    33,    42,    50,    51,    54,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    96,    97,   104,   108,   109,   112,   113,   138,   139,
     140,     6,     8,     9,    10,     6,     8,    18,    29,    55,
      56,    57,    58,    59,    60,    61,    62,    73,    74,    75,
      76,    78,   107,   116,   117,   135,   136,    18,    75,    79,
     114,   117,   118,   120,   121,   136,     7,    35,    37,    75,
      75,    75,    52,    84,     0,     3,   141,    75,    75,     8,
      75,    75,    75,   117,    34,   117,    20,    77,    78,    79,
      80,    73,    75,    79,   102,   117,    12,   107,    34,    72,
      75,   119,    37,   119,   107,    75,    75,    42,    65,    53,
      18,    72,    99,   112,    45,    75,    18,    99,    45,    19,
      75,   116,   117,   117,   117,   117,    19,    34,    19,    19,
      19,    20,   120,    20,   106,    79,    75,    20,   115,    75,
      20,   122,    19,    20,    36,    38,   125,    75,   110,   111,
      78,   107,    76,    75,   101,   112,    75,    45,    75,    75,
      75,   117,   119,    78,   107,    19,   122,   114,   136,   119,
      75,   120,   107,    18,    45,   120,   127,   128,    65,   125,
      20,   107,    35,    24,    25,    26,    27,    28,   103,    20,
      29,    31,   100,    18,    75,    20,    98,    19,    19,    37,
     107,   106,   119,   107,    20,    43,   124,    34,   119,    19,
      78,   107,   127,    30,    31,    63,    64,    65,    66,    67,
      68,    69,    70,    71,   137,    40,    41,    18,    78,   107,
     121,   110,     6,    18,   101,    29,    19,    75,    18,    75,
      19,    75,   106,   115,    75,    44,   125,    79,   122,   107,
     106,    31,    63,    64,    71,   120,   127,   127,   107,    75,
     102,   100,    99,    20,    95,    75,    98,    99,   119,   119,
      75,    46,   129,   122,   106,    19,    19,    75,    19,    95,
     124,   124,   119,    48,    47,   132,    19,    20,   105,    20,
     123,    19,   125,   127,    75,   131,    48,    39,   126,   105,
      18,    75,   129,   124,    34,    20,   130,    75,   134,   127,
      78,   107,   123,   132,    75,   131,    34,    49,    50,    20,
     133,   107,   106,   126,   130,    75,   134,   106,    19,    19,
      49,    50,   133,    19,   105,   105
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    82,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    94,    95,    95,    96,    97,    97,    97,
      97,    97,    98,    98,    99,    99,   100,   100,   101,   101,
     101,   101,   102,   103,   103,   103,   103,   103,   104,   104,
     105,   105,   105,   106,   106,   106,   107,   107,   107,   107,
     108,   109,   110,   110,   111,   111,   111,   112,   112,   113,
     114,   114,   115,   115,   116,   116,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   118,   118,   118,   119,   119,   119,   120,   120,   120,
     121,   122,   122,   122,   123,   123,   124,   124,   124,   125,
     125,   126,   126,   127,   127,   127,   127,   127,   128,   129,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     134,   134,   134,   134,   134,   135,   135,   135,   135,   135,
     136,   136,   136,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   138,   139,   140,
     140,   141,   141
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     9,    10,     0,     3,     5,     7,     4,     8,
       4,     8,     0,     3,     1,     2,     0,     3,     5,     2,
       3,     2,     1,     1,     1,     1,     1,     1,     9,    10,
       0,     6,     7,     0,     3,     4,     1,     1,     1,     1,
       4,     5,     3,     1,     3,     4,     3,    10,     4,     2,
       3,     5,     0,     4,     1,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     3,     5,     3,     3,     3,
       5,     1,     3,     4,     0,     1,     2,     1,     1,     4,
      13,     0,     4,     5,     0,     3,     0,     4,     6,     0,
       2,     0,     2,     0,     1,     3,     3,     2,     3,     0,
       4,     0,     3,     1,     3,     0,     4,     0,     3,     1,
       3,     2,     2,     4,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     1,     2,     7,     2,     4,
       5,     0,     1
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
#line 251 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1926 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 281 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1935 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 287 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1943 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 292 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1951 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 298 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1959 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 304 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1967 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 310 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1975 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 316 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1985 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 323 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1993 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 329 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2003 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE ID idx_attr_list RBRACE  */
#line 338 "yacc_sql.y"
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
#line 2024 "yacc_sql.cpp"
    break;

  case 33: /* create_index_stmt: CREATE UNIQUE INDEX ID ON ID LBRACE ID idx_attr_list RBRACE  */
#line 355 "yacc_sql.y"
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
      create_index.index_type = "UNIQUE";
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2045 "yacc_sql.cpp"
    break;

  case 34: /* idx_attr_list: %empty  */
#line 375 "yacc_sql.y"
    {
      (yyval.index_attr_list) = nullptr;
    }
#line 2053 "yacc_sql.cpp"
    break;

  case 35: /* idx_attr_list: COMMA ID rel_list  */
#line 378 "yacc_sql.y"
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.index_attr_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.index_attr_list) = new std::vector<std::string>;
      }
      (yyval.index_attr_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2067 "yacc_sql.cpp"
    break;

  case 36: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 391 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2079 "yacc_sql.cpp"
    break;

  case 37: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE  */
#line 401 "yacc_sql.y"
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
#line 2100 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID create_table_select  */
#line 418 "yacc_sql.y"
                                          {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      create_table.select_table.reset((yyvsp[0].select_sql_node));
    }
#line 2112 "yacc_sql.cpp"
    break;

  case 39: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE create_table_select  */
#line 425 "yacc_sql.y"
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
#line 2133 "yacc_sql.cpp"
    break;

  case 40: /* create_table_stmt: CREATE VIEW ID create_table_select  */
#line 441 "yacc_sql.y"
                                         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      create_table.create_view = true;
      create_table.select_table.reset((yyvsp[0].select_sql_node));
    }
#line 2146 "yacc_sql.cpp"
    break;

  case 41: /* create_table_stmt: CREATE VIEW ID LBRACE ID id_list RBRACE create_table_select  */
#line 449 "yacc_sql.y"
                                                                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));
      create_table.select_table.reset((yyvsp[0].select_sql_node));
      create_table.create_view = true;
      if ((yyvsp[-2].id_list) != nullptr) {
        (yyvsp[-2].id_list)->push_back((yyvsp[-3].string));
        free((yyvsp[-3].string));
        std::reverse((yyvsp[-2].id_list)->begin(), (yyvsp[-2].id_list)->end());
        for (std::string &str : *(yyvsp[-2].id_list)) {
          AttrInfoSqlNode attr_info;
          attr_info.name = str;
          attr_info.type = AttrType::INTS;
          attr_info.length = 4;
          create_table.attr_infos.emplace_back(attr_info);
        }
      }else {
        AttrInfoSqlNode attr_info;
        attr_info.name = (yyvsp[-3].string);
        attr_info.type = AttrType::INTS;
        attr_info.length = 4;
        create_table.attr_infos.emplace_back(attr_info);
      }
      delete (yyvsp[-2].id_list);
    }
#line 2178 "yacc_sql.cpp"
    break;

  case 42: /* id_list: %empty  */
#line 478 "yacc_sql.y"
  {
    (yyval.id_list) = nullptr;
  }
#line 2186 "yacc_sql.cpp"
    break;

  case 43: /* id_list: COMMA ID id_list  */
#line 481 "yacc_sql.y"
                     {
    if ((yyvsp[0].id_list) != nullptr) {
      (yyval.id_list) = (yyvsp[0].id_list);
    }else {
      (yyval.id_list) = new std::vector<std::string>;
    }
    (yyval.id_list)->push_back((yyvsp[-1].string));
    free((yyvsp[-1].string));
  }
#line 2200 "yacc_sql.cpp"
    break;

  case 44: /* create_table_select: select_stmt  */
#line 494 "yacc_sql.y"
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
#line 2217 "yacc_sql.cpp"
    break;

  case 45: /* create_table_select: AS select_stmt  */
#line 507 "yacc_sql.y"
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
#line 2234 "yacc_sql.cpp"
    break;

  case 46: /* attr_def_list: %empty  */
#line 522 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2242 "yacc_sql.cpp"
    break;

  case 47: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 526 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2256 "yacc_sql.cpp"
    break;

  case 48: /* attr_def: ID type LBRACE number RBRACE  */
#line 539 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      (yyval.attr_info)->not_null = true;
      free((yyvsp[-4].string));
    }
#line 2269 "yacc_sql.cpp"
    break;

  case 49: /* attr_def: ID type  */
#line 548 "yacc_sql.y"
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
#line 2286 "yacc_sql.cpp"
    break;

  case 50: /* attr_def: attr_def NOT null  */
#line 560 "yacc_sql.y"
                        {
      (yyval.attr_info) = (yyvsp[-2].attr_info);
      (yyval.attr_info)->not_null = true;
    }
#line 2295 "yacc_sql.cpp"
    break;

  case 51: /* attr_def: attr_def null  */
#line 564 "yacc_sql.y"
                    {
      (yyval.attr_info) = (yyvsp[-1].attr_info);
      (yyval.attr_info)->not_null = false;
    }
#line 2304 "yacc_sql.cpp"
    break;

  case 52: /* number: NUMBER  */
#line 570 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2310 "yacc_sql.cpp"
    break;

  case 53: /* type: INT_T  */
#line 573 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2316 "yacc_sql.cpp"
    break;

  case 54: /* type: STRING_T  */
#line 574 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2322 "yacc_sql.cpp"
    break;

  case 55: /* type: FLOAT_T  */
#line 575 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2328 "yacc_sql.cpp"
    break;

  case 56: /* type: DATE_T  */
#line 576 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2334 "yacc_sql.cpp"
    break;

  case 57: /* type: TEXT_T  */
#line 577 "yacc_sql.y"
               { (yyval.number)=TEXTS; }
#line 2340 "yacc_sql.cpp"
    break;

  case 58: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE value_lists  */
#line 581 "yacc_sql.y"
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
#line 2367 "yacc_sql.cpp"
    break;

  case 59: /* insert_stmt: INSERT INTO ID VALUES LBRACE '-' value value_list RBRACE value_lists  */
#line 603 "yacc_sql.y"
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
#line 2395 "yacc_sql.cpp"
    break;

  case 60: /* value_lists: %empty  */
#line 629 "yacc_sql.y"
  {
    (yyval.value_lists) = nullptr;
  }
#line 2403 "yacc_sql.cpp"
    break;

  case 61: /* value_lists: COMMA LBRACE value value_list RBRACE value_lists  */
#line 632 "yacc_sql.y"
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
#line 2425 "yacc_sql.cpp"
    break;

  case 62: /* value_lists: COMMA LBRACE '-' value value_list RBRACE value_lists  */
#line 649 "yacc_sql.y"
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
#line 2448 "yacc_sql.cpp"
    break;

  case 63: /* value_list: %empty  */
#line 671 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2456 "yacc_sql.cpp"
    break;

  case 64: /* value_list: COMMA value value_list  */
#line 674 "yacc_sql.y"
                             { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2470 "yacc_sql.cpp"
    break;

  case 65: /* value_list: COMMA '-' value value_list  */
#line 683 "yacc_sql.y"
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
#line 2485 "yacc_sql.cpp"
    break;

  case 66: /* value: NUMBER  */
#line 695 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2494 "yacc_sql.cpp"
    break;

  case 67: /* value: FLOAT  */
#line 699 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2503 "yacc_sql.cpp"
    break;

  case 68: /* value: SSS  */
#line 703 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
#line 2513 "yacc_sql.cpp"
    break;

  case 69: /* value: null  */
#line 708 "yacc_sql.y"
          {
      (yyval.value) = new Value();
      (yyval.value)->set_null();
      (yyloc) = (yylsp[0]);
    }
#line 2523 "yacc_sql.cpp"
    break;

  case 70: /* delete_stmt: DELETE FROM ID where  */
#line 717 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2537 "yacc_sql.cpp"
    break;

  case 71: /* update_stmt: UPDATE ID SET set_clause_list where  */
#line 729 "yacc_sql.y"
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
#line 2555 "yacc_sql.cpp"
    break;

  case 72: /* set_clause_list: set_clause COMMA set_clause_list  */
#line 745 "yacc_sql.y"
    {
      if((yyvsp[0].set_clause_list) != nullptr) {
        (yyval.set_clause_list) = (yyvsp[0].set_clause_list);
        (yyval.set_clause_list)->push_back(*(yyvsp[-2].set_clause));
      } else {
        (yyval.set_clause_list) = new std::vector<SetClauseSqlNode>;
        (yyval.set_clause_list)->push_back(*(yyvsp[-2].set_clause));
      }
    }
#line 2569 "yacc_sql.cpp"
    break;

  case 73: /* set_clause_list: set_clause  */
#line 755 "yacc_sql.y"
    {
      (yyval.set_clause_list) = new std::vector<SetClauseSqlNode>;
      (yyval.set_clause_list)->push_back(*(yyvsp[0].set_clause));
    }
#line 2578 "yacc_sql.cpp"
    break;

  case 74: /* set_clause: ID EQ value  */
#line 763 "yacc_sql.y"
    {
      (yyval.set_clause) = new SetClauseSqlNode;
      (yyval.set_clause)->attribute_name_ = (yyvsp[-2].string);
      (yyval.set_clause)->value_ = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 2590 "yacc_sql.cpp"
    break;

  case 75: /* set_clause: ID EQ '-' value  */
#line 771 "yacc_sql.y"
    {
      (yyval.set_clause) = new SetClauseSqlNode;
      modify_2_negative((yyvsp[0].value));
      (yyval.set_clause)->attribute_name_ = (yyvsp[-3].string);
      (yyval.set_clause)->value_ = *(yyvsp[0].value);
      free((yyvsp[-3].string));
      delete (yyvsp[0].value);
    }
#line 2603 "yacc_sql.cpp"
    break;

  case 76: /* set_clause: ID EQ sub_query  */
#line 779 "yacc_sql.y"
                      {
      (yyval.set_clause) = new SetClauseSqlNode;
      (yyval.set_clause)->attribute_name_ = (yyvsp[-2].string);
      (yyval.set_clause)->sub_query_.reset((yyvsp[0].sub_select));
      free((yyvsp[-2].string));
    }
#line 2614 "yacc_sql.cpp"
    break;

  case 77: /* select_stmt: SELECT select_attr FROM ID alias rel_condition_list where group order having  */
#line 790 "yacc_sql.y"
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
#line 2657 "yacc_sql.cpp"
    break;

  case 78: /* select_stmt: SELECT simple_func_item alias simple_func_list  */
#line 828 "yacc_sql.y"
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
#line 2676 "yacc_sql.cpp"
    break;

  case 79: /* calc_stmt: CALC expression_list  */
#line 846 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2687 "yacc_sql.cpp"
    break;

  case 80: /* simple_func_item: func value RBRACE  */
#line 855 "yacc_sql.y"
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
#line 2717 "yacc_sql.cpp"
    break;

  case 81: /* simple_func_item: func value COMMA value RBRACE  */
#line 880 "yacc_sql.y"
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
#line 2750 "yacc_sql.cpp"
    break;

  case 82: /* simple_func_list: %empty  */
#line 911 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 2758 "yacc_sql.cpp"
    break;

  case 83: /* simple_func_list: COMMA simple_func_item alias simple_func_list  */
#line 914 "yacc_sql.y"
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
#line 2774 "yacc_sql.cpp"
    break;

  case 84: /* expression_list: expression  */
#line 928 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<Expression*>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2783 "yacc_sql.cpp"
    break;

  case 85: /* expression_list: expression COMMA expression_list  */
#line 933 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<Expression *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2796 "yacc_sql.cpp"
    break;

  case 86: /* expression: expression '+' expression  */
#line 943 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2804 "yacc_sql.cpp"
    break;

  case 87: /* expression: expression '-' expression  */
#line 946 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2812 "yacc_sql.cpp"
    break;

  case 88: /* expression: expression '*' expression  */
#line 949 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2820 "yacc_sql.cpp"
    break;

  case 89: /* expression: expression '/' expression  */
#line 952 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2828 "yacc_sql.cpp"
    break;

  case 90: /* expression: LBRACE expression RBRACE  */
#line 955 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2837 "yacc_sql.cpp"
    break;

  case 91: /* expression: '-' expression  */
#line 959 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2845 "yacc_sql.cpp"
    break;

  case 92: /* expression: value  */
#line 962 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2855 "yacc_sql.cpp"
    break;

  case 93: /* expression: ID  */
#line 967 "yacc_sql.y"
         {
      (yyval.expression) = new FieldExpr;
      (yyval.expression)->set_name(std::string((yyvsp[0].string)));
      free((yyvsp[0].string));
    }
#line 2865 "yacc_sql.cpp"
    break;

  case 94: /* expression: ID DOT ID  */
#line 972 "yacc_sql.y"
                {
      (yyval.expression) = new FieldExpr;
      (yyval.expression)->set_name(std::string((yyvsp[-2].string)).append(".").append(std::string((yyvsp[0].string))));
      free((yyvsp[0].string));
      free((yyvsp[-2].string));
    }
#line 2876 "yacc_sql.cpp"
    break;

  case 95: /* expression: agg_func ID RBRACE  */
#line 978 "yacc_sql.y"
                         {
      FieldExpr *field_expr = new FieldExpr;
      field_expr->set_name(std::string((yyvsp[-1].string)));
      (yyval.expression) = new AggrFuncExpr((yyvsp[-2].agg_func), field_expr);
      free((yyvsp[-1].string));
    }
#line 2887 "yacc_sql.cpp"
    break;

  case 96: /* expression: agg_func ID DOT ID RBRACE  */
#line 984 "yacc_sql.y"
                                {
      FieldExpr *field_expr = new FieldExpr;
      field_expr->set_name(std::string((yyvsp[-3].string)).append(".").append(std::string((yyvsp[-1].string))));
      (yyval.expression) = new AggrFuncExpr((yyvsp[-4].agg_func), field_expr);
      free((yyvsp[-3].string));
      free((yyvsp[-1].string));
    }
#line 2899 "yacc_sql.cpp"
    break;

  case 97: /* expression: agg_func '*' RBRACE  */
#line 991 "yacc_sql.y"
                          {
      ValueExpr *expr = new ValueExpr(Value("*", 2));
      expr->set_name("*");
      (yyval.expression) = new AggrFuncExpr((yyvsp[-2].agg_func), expr);
    }
#line 2909 "yacc_sql.cpp"
    break;

  case 98: /* expression: agg_func number RBRACE  */
#line 996 "yacc_sql.y"
                             {
      ValueExpr *expr = new ValueExpr(Value((yyvsp[-1].number)));
      expr->set_name(std::to_string((yyvsp[-1].number)));
      (yyval.expression) = new AggrFuncExpr((yyvsp[-2].agg_func), expr);
    }
#line 2919 "yacc_sql.cpp"
    break;

  case 99: /* expression: func expression RBRACE  */
#line 1001 "yacc_sql.y"
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
#line 2942 "yacc_sql.cpp"
    break;

  case 100: /* expression: func expression COMMA expression RBRACE  */
#line 1019 "yacc_sql.y"
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
#line 2967 "yacc_sql.cpp"
    break;

  case 101: /* select_attr: '*'  */
#line 1042 "yacc_sql.y"
        {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
      RelAttrSqlNode attr;
      attr.relation_name  = "";
      attr.attribute_name = "*";
      (yyval.rel_attr_list)->emplace_back(attr);
    }
#line 2979 "yacc_sql.cpp"
    break;

  case 102: /* select_attr: rel_attr alias attr_list  */
#line 1049 "yacc_sql.y"
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
#line 2994 "yacc_sql.cpp"
    break;

  case 103: /* select_attr: ID DOT '*' attr_list  */
#line 1059 "yacc_sql.y"
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
#line 3010 "yacc_sql.cpp"
    break;

  case 104: /* alias: %empty  */
#line 1074 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 3018 "yacc_sql.cpp"
    break;

  case 105: /* alias: ID  */
#line 1077 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3026 "yacc_sql.cpp"
    break;

  case 106: /* alias: AS ID  */
#line 1080 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3034 "yacc_sql.cpp"
    break;

  case 107: /* rel_attr: expression  */
#line 1086 "yacc_sql.y"
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
#line 3073 "yacc_sql.cpp"
    break;

  case 108: /* rel_attr: sub_query  */
#line 1121 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->sub_query.reset((yyvsp[0].sub_select));
    }
#line 3082 "yacc_sql.cpp"
    break;

  case 109: /* rel_attr: LBRACE value value_list RBRACE  */
#line 1126 "yacc_sql.y"
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
#line 3097 "yacc_sql.cpp"
    break;

  case 110: /* sub_query: LBRACE SELECT rel_attr alias FROM ID alias rel_condition_list where group order having RBRACE  */
#line 1140 "yacc_sql.y"
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
#line 3137 "yacc_sql.cpp"
    break;

  case 111: /* attr_list: %empty  */
#line 1178 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 3145 "yacc_sql.cpp"
    break;

  case 112: /* attr_list: COMMA rel_attr alias attr_list  */
#line 1181 "yacc_sql.y"
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
#line 3160 "yacc_sql.cpp"
    break;

  case 113: /* attr_list: COMMA ID DOT '*' attr_list  */
#line 1191 "yacc_sql.y"
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
#line 3177 "yacc_sql.cpp"
    break;

  case 114: /* rel_list: %empty  */
#line 1207 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 3185 "yacc_sql.cpp"
    break;

  case 115: /* rel_list: COMMA ID rel_list  */
#line 1210 "yacc_sql.y"
                        {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 3200 "yacc_sql.cpp"
    break;

  case 116: /* rel_condition_list: %empty  */
#line 1224 "yacc_sql.y"
    {
      (yyval.relationAndConditionTempList) = nullptr;
    }
#line 3208 "yacc_sql.cpp"
    break;

  case 117: /* rel_condition_list: COMMA ID alias rel_condition_list  */
#line 1228 "yacc_sql.y"
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
#line 3224 "yacc_sql.cpp"
    break;

  case 118: /* rel_condition_list: INNER JOIN ID alias condition_list rel_condition_list  */
#line 1240 "yacc_sql.y"
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
#line 3245 "yacc_sql.cpp"
    break;

  case 119: /* where: %empty  */
#line 1260 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3253 "yacc_sql.cpp"
    break;

  case 120: /* where: WHERE condition_list  */
#line 1263 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 3261 "yacc_sql.cpp"
    break;

  case 121: /* having: %empty  */
#line 1268 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3269 "yacc_sql.cpp"
    break;

  case 122: /* having: HAVING condition_list  */
#line 1271 "yacc_sql.y"
                            {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3277 "yacc_sql.cpp"
    break;

  case 123: /* condition_list: %empty  */
#line 1277 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3285 "yacc_sql.cpp"
    break;

  case 124: /* condition_list: condition  */
#line 1280 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyvsp[0].condition)->next_or_link = 0;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 3296 "yacc_sql.cpp"
    break;

  case 125: /* condition_list: condition AND condition_list  */
#line 1286 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->next_or_link = 0;
      std::cout << "enter and" << std::endl;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 3308 "yacc_sql.cpp"
    break;

  case 126: /* condition_list: condition OR condition_list  */
#line 1293 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->next_or_link = 1;
      std::cout << "enter or" << std::endl;
      std::cout << (yyvsp[-2].condition)->next_or_link << std::endl;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      std::cout << (yyval.condition_list)->back().next_or_link << std::endl;
      delete (yyvsp[-2].condition);
    }
#line 3322 "yacc_sql.cpp"
    break;

  case 127: /* condition_list: ON condition_list  */
#line 1302 "yacc_sql.y"
                        {
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.condition_list) = (yyvsp[0].condition_list);
      }else {
        (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      }
    }
#line 3334 "yacc_sql.cpp"
    break;

  case 128: /* condition: rel_attr comp_op rel_attr  */
#line 1312 "yacc_sql.y"
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
#line 3367 "yacc_sql.cpp"
    break;

  case 129: /* group: %empty  */
#line 1344 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 3375 "yacc_sql.cpp"
    break;

  case 130: /* group: GROUP BY group_item group_list  */
#line 1348 "yacc_sql.y"
  {
    if ((yyvsp[0].rel_attr_list) == nullptr) {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }else {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
    (yyval.rel_attr_list)->emplace_back(*(yyvsp[-1].rel_attr));
    delete (yyvsp[-1].rel_attr);
  }
#line 3389 "yacc_sql.cpp"
    break;

  case 131: /* group_list: %empty  */
#line 1359 "yacc_sql.y"
  {
    (yyval.rel_attr_list) = nullptr;
  }
#line 3397 "yacc_sql.cpp"
    break;

  case 132: /* group_list: COMMA group_item group_list  */
#line 1362 "yacc_sql.y"
                                {
    if ((yyvsp[0].rel_attr_list) == nullptr) {
      (yyval.rel_attr_list) = new std::vector<RelAttrSqlNode>;
    }else {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
    }
    (yyval.rel_attr_list)->push_back(*(yyvsp[-1].rel_attr));
    delete (yyvsp[-1].rel_attr);
  }
#line 3411 "yacc_sql.cpp"
    break;

  case 133: /* group_item: ID  */
#line 1373 "yacc_sql.y"
     {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
    free((yyvsp[0].string));
  }
#line 3421 "yacc_sql.cpp"
    break;

  case 134: /* group_item: ID DOT ID  */
#line 1378 "yacc_sql.y"
              {
    (yyval.rel_attr) = new RelAttrSqlNode;
    (yyval.rel_attr)->relation_name = (yyvsp[-2].string);
    (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
    free((yyvsp[-2].string));
    free((yyvsp[0].string));
  }
#line 3433 "yacc_sql.cpp"
    break;

  case 135: /* order: %empty  */
#line 1388 "yacc_sql.y"
  {
    (yyval.order_condition_list) = nullptr;
  }
#line 3441 "yacc_sql.cpp"
    break;

  case 136: /* order: ORDER BY order_condition order_condition_list  */
#line 1391 "yacc_sql.y"
                                                  {
    if ((yyvsp[0].order_condition_list) != nullptr) {
      (yyval.order_condition_list) = (yyvsp[0].order_condition_list);
    }else {
      (yyval.order_condition_list) = new std::vector<OrderSqlNode>;
    }
    (yyval.order_condition_list)->emplace_back(*(yyvsp[-1].order_condition));
    delete (yyvsp[-1].order_condition);
  }
#line 3455 "yacc_sql.cpp"
    break;

  case 137: /* order_condition_list: %empty  */
#line 1402 "yacc_sql.y"
  {
    (yyval.order_condition_list) = nullptr;
  }
#line 3463 "yacc_sql.cpp"
    break;

  case 138: /* order_condition_list: COMMA order_condition order_condition_list  */
#line 1405 "yacc_sql.y"
                                               {
    if ((yyvsp[0].order_condition_list) != nullptr) {
      (yyval.order_condition_list) = (yyvsp[0].order_condition_list);
    }else {
      (yyval.order_condition_list) = new std::vector<OrderSqlNode>;
    }
    (yyval.order_condition_list)->emplace_back(*(yyvsp[-1].order_condition));
    delete (yyvsp[-1].order_condition);
  }
#line 3477 "yacc_sql.cpp"
    break;

  case 139: /* order_condition: ID  */
#line 1417 "yacc_sql.y"
     {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[0].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[0].string));
  }
#line 3488 "yacc_sql.cpp"
    break;

  case 140: /* order_condition: ID DOT ID  */
#line 1423 "yacc_sql.y"
              {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.relation_name = (yyvsp[-2].string);
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[0].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[-2].string));
    free((yyvsp[0].string));
  }
#line 3501 "yacc_sql.cpp"
    break;

  case 141: /* order_condition: ID ASC  */
#line 1431 "yacc_sql.y"
           {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[-1].string));
  }
#line 3512 "yacc_sql.cpp"
    break;

  case 142: /* order_condition: ID DESC  */
#line 1437 "yacc_sql.y"
            {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 0;
    free((yyvsp[-1].string));
  }
#line 3523 "yacc_sql.cpp"
    break;

  case 143: /* order_condition: ID DOT ID ASC  */
#line 1443 "yacc_sql.y"
                  {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.relation_name = (yyvsp[-3].string);
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 1;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 3536 "yacc_sql.cpp"
    break;

  case 144: /* order_condition: ID DOT ID DESC  */
#line 1451 "yacc_sql.y"
                   {
    (yyval.order_condition) = new OrderSqlNode;
    (yyval.order_condition)->attribute.relation_name = (yyvsp[-3].string);
    (yyval.order_condition)->attribute.attribute_name = (yyvsp[-1].string);
    (yyval.order_condition)->is_asc = 0;
    free((yyvsp[-3].string));
    free((yyvsp[-1].string));
  }
#line 3549 "yacc_sql.cpp"
    break;

  case 145: /* agg_func: MAX  */
#line 1460 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_MAX; }
#line 3555 "yacc_sql.cpp"
    break;

  case 146: /* agg_func: MIN  */
#line 1461 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_MIN; }
#line 3561 "yacc_sql.cpp"
    break;

  case 147: /* agg_func: COUNT  */
#line 1462 "yacc_sql.y"
            { (yyval.agg_func) = FUNC_COUNT; }
#line 3567 "yacc_sql.cpp"
    break;

  case 148: /* agg_func: AVG  */
#line 1463 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_AVG; }
#line 3573 "yacc_sql.cpp"
    break;

  case 149: /* agg_func: SUM  */
#line 1464 "yacc_sql.y"
          { (yyval.agg_func) = FUNC_SUM; }
#line 3579 "yacc_sql.cpp"
    break;

  case 150: /* func: LENGTH  */
#line 1467 "yacc_sql.y"
             { (yyval.func) = FUNC_LENGTH; }
#line 3585 "yacc_sql.cpp"
    break;

  case 151: /* func: ROUND  */
#line 1468 "yacc_sql.y"
            { (yyval.func) = FUNC_ROUND; }
#line 3591 "yacc_sql.cpp"
    break;

  case 152: /* func: DATE_FORMAT  */
#line 1469 "yacc_sql.y"
                  { (yyval.func) = FUNC_DATE_FORMAT; }
#line 3597 "yacc_sql.cpp"
    break;

  case 153: /* comp_op: EQ  */
#line 1472 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3603 "yacc_sql.cpp"
    break;

  case 154: /* comp_op: LT  */
#line 1473 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3609 "yacc_sql.cpp"
    break;

  case 155: /* comp_op: GT  */
#line 1474 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3615 "yacc_sql.cpp"
    break;

  case 156: /* comp_op: LE  */
#line 1475 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3621 "yacc_sql.cpp"
    break;

  case 157: /* comp_op: GE  */
#line 1476 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3627 "yacc_sql.cpp"
    break;

  case 158: /* comp_op: NE  */
#line 1477 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3633 "yacc_sql.cpp"
    break;

  case 159: /* comp_op: IS  */
#line 1478 "yacc_sql.y"
         { (yyval.comp) = IS_NULL; }
#line 3639 "yacc_sql.cpp"
    break;

  case 160: /* comp_op: IS NOT  */
#line 1479 "yacc_sql.y"
             { (yyval.comp) = NOT_NULL; }
#line 3645 "yacc_sql.cpp"
    break;

  case 161: /* comp_op: EXISTS  */
#line 1480 "yacc_sql.y"
             { (yyval.comp) = EXISTS_OP; }
#line 3651 "yacc_sql.cpp"
    break;

  case 162: /* comp_op: NOT EXISTS  */
#line 1481 "yacc_sql.y"
                 { (yyval.comp) = NOT_EXISTS_OP; }
#line 3657 "yacc_sql.cpp"
    break;

  case 163: /* comp_op: IN  */
#line 1482 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 3663 "yacc_sql.cpp"
    break;

  case 164: /* comp_op: NOT IN  */
#line 1483 "yacc_sql.y"
             { (yyval.comp) = NOT_IN_OP; }
#line 3669 "yacc_sql.cpp"
    break;

  case 165: /* comp_op: LIKE  */
#line 1484 "yacc_sql.y"
           { (yyval.comp) = LIKE_OP; }
#line 3675 "yacc_sql.cpp"
    break;

  case 166: /* comp_op: NOT LIKE  */
#line 1485 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE_OP; }
#line 3681 "yacc_sql.cpp"
    break;

  case 167: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1490 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3695 "yacc_sql.cpp"
    break;

  case 168: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1503 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3704 "yacc_sql.cpp"
    break;

  case 169: /* set_variable_stmt: SET ID EQ value  */
#line 1511 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3716 "yacc_sql.cpp"
    break;

  case 170: /* set_variable_stmt: SET ID EQ '-' value  */
#line 1519 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name = (yyvsp[-3].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-3].string));
      delete (yyvsp[0].value);
    }
#line 3728 "yacc_sql.cpp"
    break;


#line 3732 "yacc_sql.cpp"

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

#line 1531 "yacc_sql.y"

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

/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "zal.y"

#include <stdio.h>
#include "zal_in.h"
extern int yylex();
int
yyerror(char const *str)
{    return 0;
}

#define YYDEBUG 1

#line 82 "y.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_LITERAL = 258,
    DOUBLE_LITERAL = 259,
    STRING_LITERAL = 260,
    IDENTIFIER = 261,
    FUNCTION = 262,
    IF = 263,
    ELIF = 264,
    ELSE = 265,
    WHILE = 266,
    FOR = 267,
    RETURN = 268,
    CONTINUE = 269,
    BREAK = 270,
    NULL_T = 271,
    TRUE_T = 272,
    FALSE_T = 273,
    LP = 274,
    RP = 275,
    LC = 276,
    RC = 277,
    LB = 278,
    RB = 279,
    SEMICOLON = 280,
    COMMA = 281,
    LOGIC_AND = 282,
    LOGIC_OR = 283,
    ASSIGN = 284,
    EQ = 285,
    NE = 286,
    GE = 287,
    LE = 288,
    GT = 289,
    LT = 290,
    ADD = 291,
    SUB = 292,
    MUL = 293,
    DIV = 294,
    MOD = 295,
    INC = 296,
    DEC = 297,
    DOT = 298
  };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define STRING_LITERAL 260
#define IDENTIFIER 261
#define FUNCTION 262
#define IF 263
#define ELIF 264
#define ELSE 265
#define WHILE 266
#define FOR 267
#define RETURN 268
#define CONTINUE 269
#define BREAK 270
#define NULL_T 271
#define TRUE_T 272
#define FALSE_T 273
#define LP 274
#define RP 275
#define LC 276
#define RC 277
#define LB 278
#define RB 279
#define SEMICOLON 280
#define COMMA 281
#define LOGIC_AND 282
#define LOGIC_OR 283
#define ASSIGN 284
#define EQ 285
#define NE 286
#define GE 287
#define LE 288
#define GT 289
#define LT 290
#define ADD 291
#define SUB 292
#define MUL 293
#define DIV 294
#define MOD 295
#define INC 296
#define DEC 297
#define DOT 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "zal.y"

    char            *identifier;
    ParameterList   *para_list;
    ArgumentList    *argv_list;
    Expression      *expr;
    ExpressionList  *expr_list;
    Statement       *state;
    StatementList   *state_list;
    Block           *block;
    Elif           *elif;

#line 232 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  53
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   275

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  154

#define YYUNDEFTOK  2
#define YYMAXUTOK   298


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    46,    46,    47,    51,    52,    60,    64,    71,    75,
      76,    77,    78,    79,    80,    84,    88,    95,    99,   107,
     110,   114,   121,   125,   129,   133,   140,   147,   154,   161,
     168,   175,   179,   186,   190,   194,   198,   205,   209,   216,
     220,   227,   228,   235,   236,   243,   244,   248,   255,   256,
     260,   264,   268,   275,   276,   280,   287,   288,   292,   296,
     303,   304,   311,   312,   316,   323,   324,   325,   326,   327,
     331,   335,   339,   343,   347,   351,   355,   359,   366,   370,
     377,   381
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_LITERAL", "DOUBLE_LITERAL",
  "STRING_LITERAL", "IDENTIFIER", "FUNCTION", "IF", "ELIF", "ELSE",
  "WHILE", "FOR", "RETURN", "CONTINUE", "BREAK", "NULL_T", "TRUE_T",
  "FALSE_T", "LP", "RP", "LC", "RC", "LB", "RB", "SEMICOLON", "COMMA",
  "LOGIC_AND", "LOGIC_OR", "ASSIGN", "EQ", "NE", "GE", "LE", "GT", "LT",
  "ADD", "SUB", "MUL", "DIV", "MOD", "INC", "DEC", "DOT", "$accept",
  "script", "func_def_or_state", "func_def", "state", "para_list", "block",
  "expr_list", "if_state", "for_state", "while_state", "return_state",
  "break_state", "continue_state", "state_list", "expr", "condition",
  "elif", "l_expr", "logical_or_expr", "logical_and_expr", "eq_ne_expr",
  "compare_expr", "add_sub_expr", "math_expr", "minus_expr", "in_de_expr",
  "pri_expr", "array_literal", "argv_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298
};
# endif

#define YYPACT_NINF (-107)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-40)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     133,  -107,  -107,  -107,   -17,     1,     4,    12,    48,   238,
      91,    93,  -107,  -107,  -107,   238,   238,    16,    47,  -107,
    -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,    96,   -18,
      20,    58,    71,    56,    77,    60,  -107,  -107,    78,  -107,
     106,   109,   238,   238,   238,   117,  -107,  -107,    76,    61,
    -107,   -13,  -107,  -107,  -107,  -107,   238,   238,   128,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,  -107,  -107,  -107,  -107,   -12,    18,   127,   134,
     139,   141,   137,  -107,  -107,  -107,   209,   143,  -107,   150,
      58,    71,    56,    56,    77,    77,    77,    77,    60,    60,
    -107,  -107,  -107,  -107,   238,  -107,   159,    19,   238,   159,
     159,   238,  -107,  -107,  -107,   216,  -107,   160,  -107,   159,
     177,   158,   121,  -107,   161,  -107,    23,  -107,  -107,   187,
    -107,  -107,   166,   159,   123,   238,  -107,  -107,  -107,   238,
    -107,   168,   159,   169,   174,   238,  -107,   159,   159,   176,
    -107,  -107,   159,  -107
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    65,    66,    67,    74,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,     0,    19,     0,     0,     2,
       4,     5,     9,    10,    11,    12,    13,    14,     0,     0,
      34,    41,    43,    45,    48,    53,    56,    60,    62,    68,
       0,     0,    19,    19,     0,     0,    30,    29,     0,     0,
      20,     0,    61,     1,     3,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    70,    80,     0,     0,     0,    35,
       0,     0,     0,    28,    73,    78,     0,     0,    33,     0,
      42,    44,    46,    47,    50,    52,    49,    51,    54,    55,
      57,    58,    59,    69,     0,    15,     0,     0,     0,     0,
       0,    19,    79,    21,    40,     0,    81,     0,     7,     0,
       0,    36,    22,    27,     0,    72,     0,    18,    31,     0,
       6,    16,     0,     0,    23,     0,    71,    17,    32,    19,
      24,     0,     0,     0,     0,    19,    25,     0,     0,     0,
      26,    37,     0,    38
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -107,  -107,   170,  -107,  -100,  -107,  -106,   191,  -107,  -107,
    -107,  -107,  -107,  -107,  -107,     0,   -42,  -107,    11,  -107,
     151,   156,    94,    29,    90,    36,  -107,   194,  -107,   102
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    18,    19,    20,    21,   107,   118,    78,    22,    23,
      24,    25,    26,    27,   129,    79,    80,   134,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,    81,    40,   122,   123,    56,   -39,    41,   103,    45,
      56,    57,   -39,   130,   104,    48,    50,   128,    28,     1,
       2,     3,     4,    42,   105,    58,   -39,   140,    51,   138,
      58,    43,    12,    13,    14,    15,   146,    16,   106,   119,
      75,   150,   151,   136,    82,   120,   153,    53,    59,   104,
       1,     2,     3,     4,     5,     6,    87,    88,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    44,    16,   124,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    85,    17,    60,   113,    86,    63,    64,
      65,    66,    94,    95,    96,    97,    84,   144,    69,    70,
      71,    61,    62,   149,   116,   100,   101,   102,   121,     1,
       2,     3,     4,    67,    68,    75,    46,    28,    47,    72,
      73,    55,    12,    13,    14,    15,    74,    16,    77,    28,
     132,   133,   141,   142,    89,   143,     1,     2,     3,     4,
       5,     6,    83,    17,     7,     8,     9,    10,    11,    12,
      13,    14,    15,   108,    16,    92,    93,    98,    99,   109,
     -20,   110,   111,     1,     2,     3,     4,   114,     6,   115,
      17,     7,     8,     9,    10,    11,    12,    13,    14,    15,
     117,    16,   127,   131,   -21,   139,   135,   145,    54,   147,
       1,     2,     3,     4,   148,     6,   152,    17,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    49,    16,   137,
      90,    52,     1,     2,     3,     4,    91,   126,     0,     1,
       2,     3,     4,     0,    17,    12,    13,    14,    15,     0,
      16,   112,    12,    13,    14,    15,   125,    16,     0,     0,
       0,     1,     2,     3,     4,     0,    17,     0,     0,     0,
       0,     0,     0,    17,    12,    13,    14,    15,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17
};

static const yytype_int16 yycheck[] =
{
       0,    43,    19,   109,   110,    23,    23,     6,    20,     9,
      23,    29,    29,   119,    26,    15,    16,   117,    18,     3,
       4,     5,     6,    19,     6,    43,    43,   133,    17,   129,
      43,    19,    16,    17,    18,    19,   142,    21,    20,    20,
      40,   147,   148,    20,    44,    26,   152,     0,    28,    26,
       3,     4,     5,     6,     7,     8,    56,    57,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    19,    21,   111,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    22,    37,    27,    86,    26,    32,    33,
      34,    35,    63,    64,    65,    66,    20,   139,    38,    39,
      40,    30,    31,   145,   104,    69,    70,    71,   108,     3,
       4,     5,     6,    36,    37,   115,    25,   117,    25,    41,
      42,    25,    16,    17,    18,    19,    20,    21,    19,   129,
       9,    10,     9,    10,     6,   135,     3,     4,     5,     6,
       7,     8,    25,    37,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    26,    21,    61,    62,    67,    68,    20,
      26,    20,    25,     3,     4,     5,     6,    24,     8,    19,
      37,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      21,    21,    22,     6,    26,    19,    25,    19,    18,    20,
       3,     4,     5,     6,    20,     8,    20,    37,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    16,    21,    22,
      59,    17,     3,     4,     5,     6,    60,   115,    -1,     3,
       4,     5,     6,    -1,    37,    16,    17,    18,    19,    -1,
      21,    22,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    16,    17,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    21,    37,    45,    46,
      47,    48,    52,    53,    54,    55,    56,    57,    59,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      19,     6,    19,    19,    19,    59,    25,    25,    59,    51,
      59,    62,    71,     0,    46,    25,    23,    29,    43,    28,
      27,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    20,    59,    73,    19,    51,    59,
      60,    60,    59,    25,    20,    22,    26,    59,    59,     6,
      64,    65,    66,    66,    67,    67,    67,    67,    68,    68,
      69,    69,    69,    20,    26,     6,    20,    49,    26,    20,
      20,    25,    22,    59,    24,    19,    59,    21,    50,    20,
      26,    59,    50,    50,    60,    20,    73,    22,    48,    58,
      50,     6,     9,    10,    61,    25,    20,    22,    48,    19,
      50,     9,    10,    59,    60,    19,    50,    20,    20,    60,
      50,    50,    20,    50
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      48,    48,    48,    48,    48,    49,    49,    50,    50,    51,
      51,    51,    52,    52,    52,    52,    53,    54,    55,    56,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      66,    66,    66,    67,    67,    67,    68,    68,    68,    68,
      69,    69,    70,    70,    70,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    72,    72,
      73,    73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     6,     5,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     0,
       1,     3,     5,     6,     7,     8,     9,     5,     3,     2,
       2,     1,     2,     3,     1,     1,     3,     5,     6,     1,
       4,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     2,     1,     2,     2,     1,     1,     1,     1,     4,
       3,     6,     5,     3,     1,     1,     1,     1,     3,     4,
       1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
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
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
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
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
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
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 5:
#line 53 "zal.y"
    {
        ZAL_Interpreter* inter = zal_get_current_inter();
        inter->state_list = zal_add_statement_to_list(inter->state_list, (yyvsp[0].state));
    }
#line 1542 "y.tab.c"
    break;

  case 6:
#line 61 "zal.y"
    {
        zal_inter_create_function((yyvsp[-4].identifier), (yyvsp[-2].para_list), (yyvsp[0].block));
    }
#line 1550 "y.tab.c"
    break;

  case 7:
#line 65 "zal.y"
    {
        zal_inter_create_function((yyvsp[-3].identifier), NULL, (yyvsp[0].block));
    }
#line 1558 "y.tab.c"
    break;

  case 8:
#line 72 "zal.y"
    {
        (yyval.state) = zal_create_expr_statement((yyvsp[-1].expr));
    }
#line 1566 "y.tab.c"
    break;

  case 15:
#line 85 "zal.y"
    {
        (yyval.para_list) = zal_create_parameter_list((yyvsp[0].identifier));
    }
#line 1574 "y.tab.c"
    break;

  case 16:
#line 89 "zal.y"
    {
        (yyval.para_list) = zal_add_parameter_to_list((yyvsp[-2].para_list), (yyvsp[0].identifier));
    }
#line 1582 "y.tab.c"
    break;

  case 17:
#line 96 "zal.y"
    {
        (yyval.block) = zal_create_block((yyvsp[-1].state_list));
    }
#line 1590 "y.tab.c"
    break;

  case 18:
#line 100 "zal.y"
    {
        (yyval.block) = zal_create_block(NULL);
    }
#line 1598 "y.tab.c"
    break;

  case 19:
#line 107 "zal.y"
    {
        (yyval.expr_list) = NULL;
    }
#line 1606 "y.tab.c"
    break;

  case 20:
#line 111 "zal.y"
    {
        (yyval.expr_list) = zal_create_expr_list((yyvsp[0].expr));
    }
#line 1614 "y.tab.c"
    break;

  case 21:
#line 115 "zal.y"
    {
        (yyval.expr_list) = zal_add_expr_to_list((yyvsp[-2].expr_list), (yyvsp[0].expr));
    }
#line 1622 "y.tab.c"
    break;

  case 22:
#line 122 "zal.y"
    {
        (yyval.state) = zal_create_if_statement((yyvsp[-2].expr), (yyvsp[0].block), NULL, NULL);
    }
#line 1630 "y.tab.c"
    break;

  case 23:
#line 126 "zal.y"
    {
        (yyval.state) = zal_create_if_statement((yyvsp[-3].expr), (yyvsp[-1].block), (yyvsp[0].elif), NULL);
    }
#line 1638 "y.tab.c"
    break;

  case 24:
#line 130 "zal.y"
    {
        (yyval.state) = zal_create_if_statement((yyvsp[-4].expr), (yyvsp[-2].block), NULL, (yyvsp[0].block));
    }
#line 1646 "y.tab.c"
    break;

  case 25:
#line 134 "zal.y"
    {
        (yyval.state) = zal_create_if_statement((yyvsp[-5].expr), (yyvsp[-3].block), (yyvsp[-2].elif), (yyvsp[0].block));
    }
#line 1654 "y.tab.c"
    break;

  case 26:
#line 141 "zal.y"
    {
        (yyval.state) = zal_create_for_statement((yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].block));
    }
#line 1662 "y.tab.c"
    break;

  case 27:
#line 148 "zal.y"
    {
        (yyval.state) = zal_create_while_statement((yyvsp[-2].expr), (yyvsp[0].block));
    }
#line 1670 "y.tab.c"
    break;

  case 28:
#line 155 "zal.y"
    {
        (yyval.state) = zal_create_return_statement((yyvsp[-1].expr));
    }
#line 1678 "y.tab.c"
    break;

  case 29:
#line 162 "zal.y"
    {
        (yyval.state) = zal_create_break_statement();
    }
#line 1686 "y.tab.c"
    break;

  case 30:
#line 169 "zal.y"
    {
        (yyval.state) = zal_create_continue_statement();
    }
#line 1694 "y.tab.c"
    break;

  case 31:
#line 176 "zal.y"
    {
        (yyval.state_list) = zal_create_statement_list((yyvsp[0].state));
    }
#line 1702 "y.tab.c"
    break;

  case 32:
#line 180 "zal.y"
    {
        (yyval.state_list) = zal_add_statement_to_list((yyvsp[-1].state_list), (yyvsp[0].state));
    }
#line 1710 "y.tab.c"
    break;

  case 33:
#line 187 "zal.y"
    {
        (yyval.expr) = zal_create_assign_expr((yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1718 "y.tab.c"
    break;

  case 35:
#line 195 "zal.y"
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 1726 "y.tab.c"
    break;

  case 36:
#line 199 "zal.y"
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 1734 "y.tab.c"
    break;

  case 37:
#line 206 "zal.y"
    {
        (yyval.elif) = zal_create_elif((yyvsp[-2].expr), (yyvsp[0].block));
    }
#line 1742 "y.tab.c"
    break;

  case 38:
#line 210 "zal.y"
    {
        (yyval.elif) = zal_add_elif_to_list((yyvsp[-5].elif), (yyvsp[-2].expr), (yyvsp[0].block));
    }
#line 1750 "y.tab.c"
    break;

  case 39:
#line 217 "zal.y"
    {
        (yyval.expr) = zal_create_identifier_expr((yyvsp[0].identifier));
    }
#line 1758 "y.tab.c"
    break;

  case 40:
#line 221 "zal.y"
    {
        (yyval.expr) = zal_create_index_expr((yyvsp[-3].expr), (yyvsp[-1].expr));
    }
#line 1766 "y.tab.c"
    break;

  case 42:
#line 229 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(LOGICAL_OR_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1774 "y.tab.c"
    break;

  case 44:
#line 237 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(LOGICAL_AND_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1782 "y.tab.c"
    break;

  case 46:
#line 245 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(EQ_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1790 "y.tab.c"
    break;

  case 47:
#line 249 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(NE_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1798 "y.tab.c"
    break;

  case 49:
#line 257 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(GT_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1806 "y.tab.c"
    break;

  case 50:
#line 261 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(GE_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1814 "y.tab.c"
    break;

  case 51:
#line 265 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(LT_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1822 "y.tab.c"
    break;

  case 52:
#line 269 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(LE_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1830 "y.tab.c"
    break;

  case 54:
#line 277 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(ADD_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1838 "y.tab.c"
    break;

  case 55:
#line 281 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(SUB_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1846 "y.tab.c"
    break;

  case 57:
#line 289 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(MUL_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1854 "y.tab.c"
    break;

  case 58:
#line 293 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(DIV_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1862 "y.tab.c"
    break;

  case 59:
#line 297 "zal.y"
    {
        (yyval.expr) = zal_create_binary_expr(MOD_EXPRESSION, (yyvsp[-2].expr), (yyvsp[0].expr));
    }
#line 1870 "y.tab.c"
    break;

  case 61:
#line 305 "zal.y"
    {
        (yyval.expr) = zal_create_minus_expr((yyvsp[0].expr));
    }
#line 1878 "y.tab.c"
    break;

  case 63:
#line 313 "zal.y"
    {
        (yyval.expr) = zal_create_inc_dec_expr((yyvsp[-1].expr), INCREMENT_EXPRESSION);
    }
#line 1886 "y.tab.c"
    break;

  case 64:
#line 317 "zal.y"
    {
        (yyval.expr) = zal_create_inc_dec_expr((yyvsp[-1].expr), DECREMENT_EXPRESSION);
    }
#line 1894 "y.tab.c"
    break;

  case 69:
#line 328 "zal.y"
    {
        (yyval.expr) = zal_create_func_call_expr((yyvsp[-3].identifier), (yyvsp[-1].argv_list));
    }
#line 1902 "y.tab.c"
    break;

  case 70:
#line 332 "zal.y"
    {
        (yyval.expr) = zal_create_func_call_expr((yyvsp[-2].identifier), NULL);
    }
#line 1910 "y.tab.c"
    break;

  case 71:
#line 336 "zal.y"
    {
        (yyval.expr) = zal_create_method_call_expr((yyvsp[-5].expr), (yyvsp[-3].identifier), (yyvsp[-1].argv_list));
    }
#line 1918 "y.tab.c"
    break;

  case 72:
#line 340 "zal.y"
    {
        (yyval.expr) = zal_create_method_call_expr((yyvsp[-4].expr), (yyvsp[-2].identifier), NULL);
    }
#line 1926 "y.tab.c"
    break;

  case 73:
#line 344 "zal.y"
    {
        (yyval.expr) = (yyvsp[-1].expr);
    }
#line 1934 "y.tab.c"
    break;

  case 74:
#line 348 "zal.y"
    {
        (yyval.expr) = zal_create_identifier_expr((yyvsp[0].identifier));
    }
#line 1942 "y.tab.c"
    break;

  case 75:
#line 352 "zal.y"
    {
        (yyval.expr) = zal_create_null_expr();
    }
#line 1950 "y.tab.c"
    break;

  case 76:
#line 356 "zal.y"
    {
        (yyval.expr) = zal_create_bool_expr(ZAL_TRUE);
    }
#line 1958 "y.tab.c"
    break;

  case 77:
#line 360 "zal.y"
    {
        (yyval.expr) = zal_create_bool_expr(ZAL_FALSE);
    }
#line 1966 "y.tab.c"
    break;

  case 78:
#line 367 "zal.y"
    {
        (yyval.expr) = zal_create_array_expr((yyvsp[-1].expr_list));
    }
#line 1974 "y.tab.c"
    break;

  case 79:
#line 371 "zal.y"
    {
        (yyval.expr) = zal_create_array_expr((yyvsp[-2].expr_list));
    }
#line 1982 "y.tab.c"
    break;

  case 80:
#line 378 "zal.y"
    {
        (yyval.argv_list) = zal_create_argument_list((yyvsp[0].expr));
    }
#line 1990 "y.tab.c"
    break;

  case 81:
#line 382 "zal.y"
    {
        (yyval.argv_list) = zal_add_argument_to_list((yyvsp[-2].argv_list), (yyvsp[0].expr));
    }
#line 1998 "y.tab.c"
    break;


#line 2002 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
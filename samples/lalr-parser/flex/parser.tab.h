/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_DEF = 258,
    T_INTTYPE = 259,
    T_BOOLTYPE = 260,
    T_FLOATTYPE = 261,
    T_STRINGTYPE = 262,
    T_CHARTYPE = 263,
    T_ARROR = 264,
    T_EQUAL = 265,
    T_NOTEQUAL = 266,
    T_AND = 267,
    T_OR = 268,
    T_LESSEREQUAL = 269,
    T_GREATEREQUAL = 270,
    T_LESSER = 271,
    T_GREATER = 272,
    T_MUL = 273,
    T_DIV = 274,
    T_MOD = 275,
    T_ADD = 276,
    T_SUB = 277,
    T_ASSIGN = 278,
    T_SQSTART = 279,
    T_SQEND = 280,
    T_PARSTART = 281,
    T_PAREND = 282,
    T_EXMARK = 283,
    T_COMMA = 284,
    T_PIPE = 285,
    T_ARROW = 286,
    T_COLON = 287,
    T_INTLITERAL = 288,
    T_ID = 289,
    T_STRINGLITERAL = 290,
    T_CHARLITERAL = 291,
    T_FLOATLITERAL = 292,
    T_BOOLLITERAL = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 11 "parser.y" /* yacc.c:1909  */

    string*	str_val;
    long	long_val;
    long double	longdouble_val;
    bool	bool_val;

#line 100 "parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

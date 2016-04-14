%skeleton "lalr1.cc"
%name-prefix "parser"
%define "parser_class_name" {Parser}
%locations
%initial-action {
    @$.begin = new Location();
    @$.end = new Location();
    //@$.begin->source = @$.end->source = driver.source;
}
%debug
%parse-param { class Driver& driver }
%error-verbose

%code requires {
#include "Common.h"

using namespace common;
using namespace std;

#include "Variant.h"

}

%{
#include <string>
#include <vector>

%}

%define api.value.type {Variant}
%define api.location.type {common::Location}


%token END 0 "end of stream"
%token EOL "end of line"
%token DEF INTTYPE BOOLTYPE FLOATTYPE STRINGTYPE CHARTYPE ARROR EQUAL NOTEQUAL AND OR LESSEREQUAL GREATEREQUAL LESSER GREATER MUL DIV MOD ADD SUB ASSIGN SQSTART SQEND PARSTART PAREND EXMARK COMMA PIPE ARROW COLON
%token <long_int> INTLITERAL
%token <chr> CHARLITERAL
%token <boolean> BOOLLITERAL
%token <long_double> FLOATLITERAL
%token <str> ID STRINGLITERAL

%left OR
%left AND
%left EQUAL NOTEQUAL
%left LESSER GREATER GREATEREQUAL LESSEREQUAL
%left ADD SUB
%left MUL DIV MOD
%right COLON
%precedence EXMARK

%type <func> func decl
%type <func_vec> funcs_ne
%type <cse> case
%type <cse_vec> cases_ne
%type <pattern> pattern literal
%type <pattern_vec> patterns patterns_comma_ne patterns_comma
%type <expr> expr struct_inst
%type <expr_vec> exprs_comma exprs_comma_ne
%type <type> type signature types_comma_ne

%{

#include "Driver.h"
#include "Scanner.h"

// Connect bison parser to flex scanner
#undef yylex
#define yylex driver.lexer->lex

#define release(ITEM) move(* ITEM.release())

#define force_copy(DIST, SRC) memcpy(&DIST, &SRC, sizeof(decltype(SRC)))

#define make(DIST, TYPE, ARGS) memset(&DIST, 0, sizeof(unique_ptr<TYPE>)); DIST = make_unique<TYPE>ARGS

%}


%start program

%%

program:	funcs_ne                                        { driver.program = make_unique<Program>(release($1), @1); }
    |       expr                                            { driver.program = make_unique<Program>(move($1), @1); };
funcs_ne:	funcs_ne func                                   { $$ = move($1); $$->push_back(move($2)); }
	|       func                                            { make($$, vector<unique_ptr<Function>>, ()); $$->push_back(move($1)); } ;
//func:		decl cases_ne                                   { $$ = move($1); move_uniq_vec($2, $$->cases); }
func:		decl cases_ne                                   { $$ = move($1); $$->cases = release($2); }
decl:		DEF ID COLON signature                          { make($$, Function, (* $2, release($4), @1)); }
signature:	signature ARROR type                            { $$ = move($1); $$->subtypes.push_back(* $3); }
	|       type                                            { make($$, Type, (TypeId::SIGNATURE, @1)); $$->subtypes.push_back(* $1); } ;
type:	BOOLTYPE                                            { make($$, Type, (TypeId::BOOL, @1)); }
	|	INTTYPE                                             { make($$, Type, (TypeId::INT, @1)); }
	|	FLOATTYPE                                           { make($$, Type, (TypeId::FLOAT, @1)); }
	|	CHARTYPE                                            { make($$, Type, (TypeId::CHAR, @1)); }
	|	STRINGTYPE                                          { make($$, Type, (TypeId::STRING, @1)); }
	|	SQSTART type SQEND                                  { make($$, Type, (TypeId::LIST, @2)); $$->subtypes.push_back(* $2); }
	|	PARSTART signature PAREND                           { $$ = move($2); }
	|	PARSTART types_comma_ne COMMA type PAREND           { make($$, Type, (TypeId::TUPLE, $2->subtypes, @2)); $$->subtypes.push_back(* $4); }
types_comma_ne: types_comma_ne COMMA type                   { $$ = move($1); $$->subtypes.push_back(* $3); }
	|	type                                                { make($$, Type, ()); $$->subtypes.push_back(* $1); };
cases_ne:	cases_ne case                                   { $$ = move($1); $$->push_back(move($2)); }
	|   case                                                { make($$, vector<unique_ptr<Case>>, ()); $$->push_back(move($1)); };
case: 		PIPE patterns ASSIGN expr                       { make($$, Case, (move($4), release($2), @2)); }
patterns:	patterns pattern                                { $$ = move($1); $$->push_back(move($2)); }
	|                                                       { make($$, vector<unique_ptr<Pattern>>, ()); } ;
pattern:    literal                                         { $$ = move($1); }
	| 	ID                                                  { make($$, Id, (* $1, @1)); }
	| 	PARSTART pattern COLON pattern PAREND               { make($$, ListSplit, (move($2), move($4), @2));  }
	| 	PARSTART patterns_comma_ne COMMA pattern PAREND     { $2->push_back(move($4)); make($$, TuplePattern, (release($2), @2));  }
    |   SQSTART patterns_comma SQEND                        { make($$, ListPattern, (release($2), @2)); }
patterns_comma:    patterns_comma_ne                        { $$ = move($1); }
	|	                                                    { make($$, vector<unique_ptr<Pattern>>, ()); } ;
patterns_comma_ne:  patterns_comma_ne COMMA pattern         { $$ = move($1); $$->push_back(move($3)); }
	|   pattern                                             { make($$, vector<unique_ptr<Pattern>>, ()); $$->push_back(move($1)); } ;
literal:	INTLITERAL                                      { make($$, Int,($1, @1)); }
	|	SUB INTLITERAL                                      { make($$, Int, (- $2, @2)); }
	|	FLOATLITERAL                                        { make($$, Float, ($1, @1)); }
	|	SUB FLOATLITERAL                                    { make($$, Float, (- $2, @2)); }
	|	CHARLITERAL                                         { make($$, Char, ($1, @1)); }
	|	STRINGLITERAL                                       { make($$, String, (* $1, @1)); }
	|	BOOLLITERAL                                         { make($$, Bool, ($1, @1)); } ;
expr:	expr OR expr                                        { make($$, Or, (move($1), move($3), @2)); }
	|	expr AND expr                                       { make($$, And, (move($1), move($3), @2)); }
	|	expr EQUAL expr                                     { make($$, Equal, (move($1), move($3), @2)); }
	|	expr NOTEQUAL expr                                  { make($$, NotEqual, (move($1), move($3), @2)); }
	|	expr LESSER expr                                    { make($$, Lesser, (move($1), move($3), @2)); }
	|	expr GREATER expr                                   { make($$, Greater, (move($1), move($3), @2)); }
	|	expr LESSEREQUAL expr                               { make($$, LesserEq, (move($1), move($3), @2)); }
	|	expr GREATEREQUAL expr                              { make($$, GreaterEq, (move($1), move($3), @2)); }
	|	expr ADD expr                                       { make($$, Add, (move($1), move($3), @2)); }
	|	expr SUB expr                                       { make($$, Sub, (move($1), move($3), @2)); }
	|	expr MUL expr                                       { make($$, Mul, (move($1), move($3), @2)); }
	|	expr DIV expr                                       { make($$, Div, (move($1), move($3), @2)); }
	|	expr MOD expr                                       { make($$, Mod, (move($1), move($3), @2)); }
	|	expr COLON expr                                     { make($$, ListAdd, (move($1), move($3), @2)); }
	|	ID                                                  { make($$, Id, (* $1, @1)); }
	|	literal                                             { force_copy($$, $1); }
	|	struct_inst                                         { $$ = move($1); }
	|	PARSTART expr PAREND                                { make($$, Par, (move($2), @1)); }
	|	expr PARSTART exprs_comma PAREND                    { make($$, Call, (move($1), release($3), @2)); }
	|	EXMARK expr                                         { make($$, Not, (move($2), @1)); } ;
struct_inst:	SQSTART exprs_comma SQEND                   { make($$, List, (release($2), @1)); }
	|	PARSTART exprs_comma_ne COMMA expr PAREND           { $2->push_back(move($4)); make($$, Tuple, (release($2), @1)); }  ;
exprs_comma:    exprs_comma_ne                              { $$ = move($1); }
	|	                                                    { make($$, vector<unique_ptr<Expr>>, ()); } ;
exprs_comma_ne: exprs_comma_ne COMMA expr                   { $$ = move($1); $$->push_back(move($3));  }
	|	expr                                                { make($$, vector<unique_ptr<Expr>>, ()); $$->push_back(move($1)); } ;

%%

void parser::Parser::error(const Parser::location_type& loc,
                           const std::string& msg)
{
    driver.error(loc, msg);
}

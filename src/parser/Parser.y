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

//%define api.value.type {Variant}
%union {
    char chr;
    long long_int;
    long double long_double;
    bool boolean;

    Type *      type;
    Function *  func;
    Case *      cse;
    Pattern *   pattern;
    Expr *      expr;
    string *    str;

    vector<unique_ptr<Function>> *  func_vec;
    vector<unique_ptr<Case>> *      cse_vec;
    vector<unique_ptr<Pattern>> *   pattern_vec;
    vector<unique_ptr<Expr>> *      expr_vec;
}

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
#define yylex driver.scanner->lex


%}


%start program

%%

program:	funcs_ne                                        { driver.program = make_unique<Program>(move(* $1), @1); delete $1; }
    |       expr                                            { driver.program = make_unique<Program>(unique_ptr<Expr>($1), @1); }
funcs_ne:	funcs_ne func                                   { $$ = $1; $$->push_back(unique_ptr<Function>($2)); }
	|       func                                            { $$ = new vector<unique_ptr<Function>>(); $$->push_back(unique_ptr<Function>($1)); }
func:		decl cases_ne                                   { $$ = $1; $$->cases = move(* $2); delete $2; }
decl:		DEF ID COLON signature                          { $$ = new Function(* $2, * $4, @1); }
signature:	signature ARROR type                            { $$ = $1; $$->subtypes.push_back(* $3); }
	|       type                                            { $$ = new Type(TypeId::SIGNATURE, @1); $$->subtypes.push_back(* $1); }
type:	BOOLTYPE                                            { $$ = new Type(TypeId::BOOL, @1); }
	|	INTTYPE                                             { $$ = new Type(TypeId::INT, @1); }
	|	FLOATTYPE                                           { $$ = new Type(TypeId::FLOAT, @1); }
	|	CHARTYPE                                            { $$ = new Type(TypeId::CHAR, @1); }
	|	STRINGTYPE                                          { $$ = new Type(TypeId::STRING, @1); }
	|	SQSTART type SQEND                                  { $$ = new Type(TypeId::LIST, @2); $$->subtypes.push_back(* $2); }
	|	PARSTART signature PAREND                           { $$ = $2; }
	|	PARSTART types_comma_ne COMMA type PAREND           { $$ = new Type(TypeId::TUPLE, $2->subtypes, @2); $$->subtypes.push_back(* $4); }
types_comma_ne: types_comma_ne COMMA type                   { $$ = $1; $$->subtypes.push_back(* $3); }
	|	type                                                { $$ = new Type(); $$->subtypes.push_back(* $1); }
cases_ne:	cases_ne case                                   { $$ = $1; $$->push_back(unique_ptr<Case>($2)); }
	|   case                                                { $$ = new vector<unique_ptr<Case>>(); $$->push_back(unique_ptr<Case>($1)); }
case: 		PIPE patterns ASSIGN expr                       { $$ = new Case(unique_ptr<Expr>($4), move(* $2), @2); delete $2; }
patterns:	patterns pattern                                { $$ = $1; $$->push_back(unique_ptr<Pattern>($2)); }
	|                                                       { $$ = new vector<unique_ptr<Pattern>>(); }
pattern:    literal                                         { $$ = $1; }
	| 	ID                                                  { $$ = new Id(* $1, @1); }
	| 	PARSTART pattern COLON pattern PAREND               { $$ = new ListSplit(unique_ptr<Pattern>($2), unique_ptr<Pattern>($4), @2);  }
	| 	PARSTART patterns_comma_ne COMMA pattern PAREND     { $2->push_back(unique_ptr<Pattern>($4)); $$ = new TuplePattern(move(* $2), @2); delete $2; }
    |   SQSTART patterns_comma SQEND                        { $$ = new ListPattern(move(* $2), @2); delete $2; }
patterns_comma:    patterns_comma_ne                        { $$ = $1; }
	|	                                                    { $$ = new vector<unique_ptr<Pattern>>(); }
patterns_comma_ne:  patterns_comma_ne COMMA pattern         { $$ = $1; $$->push_back(unique_ptr<Pattern>($3)); }
	|   pattern                                             { $$ = new vector<unique_ptr<Pattern>>(); $$->push_back(unique_ptr<Pattern>($1)); }
literal:	INTLITERAL                                      { $$ = new Int($1, @1); }
	|	SUB INTLITERAL                                      { $$ = new Int(- $2, @2); }
	|	FLOATLITERAL                                        { $$ = new Float($1, @1); }
	|	SUB FLOATLITERAL                                    { $$ = new Float(- $2, @2); }
	|	CHARLITERAL                                         { $$ = new Char($1, @1); }
	|	STRINGLITERAL                                       { $$ = new String(* $1, @1); }
	|	BOOLLITERAL                                         { $$ = new Bool($1, @1); }
expr:	expr OR expr                                        { $$ = new Or(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr AND expr                                       { $$ = new And(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr EQUAL expr                                     { $$ = new Equal(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr NOTEQUAL expr                                  { $$ = new NotEqual(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr LESSER expr                                    { $$ = new Lesser(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr GREATER expr                                   { $$ = new Greater(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr LESSEREQUAL expr                               { $$ = new LesserEq(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr GREATEREQUAL expr                              { $$ = new GreaterEq(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr ADD expr                                       { $$ = new Add(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr SUB expr                                       { $$ = new Sub(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr MUL expr                                       { $$ = new Mul(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr DIV expr                                       { $$ = new Div(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr MOD expr                                       { $$ = new Mod(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	expr COLON expr                                     { $$ = new ListAdd(unique_ptr<Expr>($1), unique_ptr<Expr>($3), @2); }
	|	ID                                                  { $$ = new Id(* $1, @1); }
	|	literal                                             { $$ = $1; }
	|	struct_inst                                         { $$ = $1; }
	|	PARSTART expr PAREND                                { $$ = new Par(unique_ptr<Expr>($2), @1); }
	|	expr PARSTART exprs_comma PAREND                    { $$ = new Call(unique_ptr<Expr>($1), move(* $3), @2); delete $3; }
	|	EXMARK expr                                         { $$ = new Not(unique_ptr<Expr>($2), @1); }
struct_inst:	SQSTART exprs_comma SQEND                   { $$ = new List(move(* $2), @1); delete $2; }
	|	PARSTART exprs_comma_ne COMMA expr PAREND           { $2->push_back(unique_ptr<Expr>($4)); $$ = new Tuple(move(* $2), @1); delete $2; }
exprs_comma:    exprs_comma_ne                              { $$ = $1; }
	|	                                                    { $$ = new vector<unique_ptr<Expr>>(); }
exprs_comma_ne: exprs_comma_ne COMMA expr                   { $$ = $1; $$->push_back(unique_ptr<Expr>($3));  }
	|	expr                                                { $$ = new vector<unique_ptr<Expr>>(); $$->push_back(unique_ptr<Expr>($1)); }

%%

void parser::Parser::error(const Parser::location_type& loc,
                           const std::string& msg)
{
    driver.error(loc, msg);
}

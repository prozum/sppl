%name-prefix "parser"
%define "parser_class_name" {Parser}
%locations
%define api.location.type {Location}
%initial-action {
    @$.begin.Src = @$.end.Src = Drv.Source;
}
%debug
%parse-param { class Driver &Drv }
%error-verbose

// Parser.h
%code requires {
#include "Common.h"

#include <string>
#include <vector>

using namespace common;
using namespace std;

}

// Parser.cpp
%{
#include "Driver.h"

// Connect bison parser to flex scanner
#undef yylex
#define yylex Drv.Snr.lex
%}

%union {
    char chr;
    long long_int;
    long double long_double;
    bool boolean;

    Type *          type;
    Function *      func;
    Case *          cse;
    Pattern *       pattern;
    Expression *    expr;
    string *        str;

    vector<unique_ptr<Function>> *      func_vec;
    vector<unique_ptr<Case>> *          cse_vec;
    vector<unique_ptr<Pattern>> *       pattern_vec;
    vector<unique_ptr<Expression>> *    expr_vec;
}

%token END 0 "End"
%token EOL "End of line"
%token DEF "def"
%token TYPE "type"
%token BACKSLASH "\\"
%token INCLUDE "Include"
%token TO "to"
%token WILD "_"
%token CONCAT "++"
%token PROCON "|>"
%token INTTYPE "Int Type"
%token BOOLTYPE "Bool Type"
%token FLOATTYPE "Float Type"
%token STRINGTYPE "String Type"
%token CHARTYPE "Char Type"
%token LAMBARROW "=>"
%token ARROR "->"
%token EQUAL "=="
%token NOTEQUAL "!="
%token AND "&&"
%token OR "||"
%token LESSEREQUAL "<="
%token GREATEREQUAL ">="
%token LESSER "<"
%token GREATER ">"
%token MUL "*"
%token DIV "/"
%token MOD "%"
%token ADD "+"
%token SUB "-"
%token ASSIGN "="
%token SQSTART "["
%token SQEND "]"
%token PARSTART "("
%token PAREND ")"
%token EXMARK "!"
%token COMMA ","
%token PIPE "|"
%token COLON ":"
%token <long_int> INTLITERAL "Int"
%token <chr> CHARLITERAL "Char"
%token <boolean> BOOLLITERAL "Bool"
%token <long_double> FLOATLITERAL "Float"
%token <str> ID STRINGLITERAL "String"

%left OR
%left AND
%left EQUAL NOTEQUAL
%left LESSER GREATER GREATEREQUAL LESSEREQUAL
%left ADD SUB
%left MUL DIV MOD
%left PROCON
%left CONCAT
%right COLON
%left TO
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

// Start at program
%start program

%%

program:	includes decls                                  { Drv.Prog = make_unique<Program>(move(* $1), @1); delete $1; }
    |       expr                                            { Drv.Prog = make_unique<Program>(unique_ptr<Expression>($1), @1); }
includes: includes include                                  { /* TODO Functionality for include! */ }
    |                                                       { /* TODO Functionality for include! */ }
include:    INCLUDE STRINGLITERAL                           { /* TODO Functionality for include! */ }
decls:      decls decl                                      { $$ = $1; $$->push_back(unique_ptr<Expression>($2); }
    |                                                       { $$ = new vector<unique_ptr<Declaration>>() }
decl:       func                                            { $$ = $1; }
    |       adt                                             { $$ = $1 }
func:		DEF ID COLON signature cases_ne                 { $$ = new Function(* $2, * $4, @1); $$->Cases = move(* $5); delete $2; }
adt:        TYPE ID types ASSIGN options                    { /* TODO Implement ADT */ }
types:      types type                                      { /* TODO Implement the types, for ADT */ }
    |                                                       { /* TODO Implement the types, for ADT */ }
sum:        sum PIPE product                                { /* TODO Implement sum for ADT */ }
    |       product                                         { /* TODO Implement sum for ADT */ }
product:    product value                                   { /* TODO Implement product for ADT */ }
    |       value                                           { /* TODO Implement product for ADT */ }
value:      ID                                              { /* TODO Implement value for ADT */ }
    |       type                                            { /* TODO Implement value for ADT */ }
signature:	signature ARROR type                            { $$ = $1; $$->Subtypes.push_back(* $3); }
	|       type                                            { $$ = new Type(TypeId::SIGNATURE, @1); $$->Subtypes.push_back(* $1); }
type:	    INTTYPE                                         { $$ = new Type(TypeId::INT, @1); }
	|	    FLOATTYPE                                       { $$ = new Type(TypeId::FLOAT, @1); }
	|	    CHARTYPE                                        { $$ = new Type(TypeId::CHAR, @1); }
	|	    SQSTART type SQEND                              { $$ = new Type(TypeId::LIST, @2); $$->Subtypes.push_back(* $2); }
	|	    PARSTART signature PAREND                       { $$ = $2; }
	|	    PARSTART tupletypes COMMA type PAREND           { $$ = new Type(TypeId::TUPLE, $2->Subtypes, @2); $$->Subtypes.push_back(* $4); }
	|       LESSER ID GREATER                               { /* TODO Functionality for generics */ }
tupletypes: types_comma_ne COMMA type                       { $$ = $1; $$->Subtypes.push_back(* $3); }
	|	    type                                            { $$ = new Type(); $$->Subtypes.push_back(* $1); }
cases_ne:	cases_ne case                                   { $$ = $1; $$->push_back(unique_ptr<Case>($2)); }
	|       case                                            { $$ = new vector<unique_ptr<Case>>(); $$->push_back(unique_ptr<Case>($1)); }
case: 		PIPE patterns ASSIGN expr                       { $$ = new Case(unique_ptr<Expression>($4), nullptr, move(* $2), @2); delete $2; }
    |		PIPE patterns WHEN expr ASSIGN expr             { $$ = new Case(unique_ptr<Expression>($6), unique_ptr<Expression>($4), move(* $2), @2); delete $2; }
patterns:	patterns pattern                                { $$ = $1; $$->push_back(unique_ptr<Pattern>($2)); }
	|                                                       { $$ = new vector<unique_ptr<Pattern>>(); }
pattern:    INTLITERAL                                      { $$ = new IntPattern($1, @1); }
    |	    SUB INTLITERAL                                  { $$ = new IntPattern(- $2, @2); }
    |	    FLOATLITERAL                                    { $$ = new FloatPattern($2, @2); }
    |	    SUB FLOATLITERAL                                { $$ = new FloatPattern(- $2, @2); }
	|	    CHARLITERAL                                     { $$ = new CharPattern($1, @1); }
	|	    STRINGLITERAL                                   { /* TODO STRINGLITERALS should have a pattern */  }
	| 	    ID                                              { $$ = new Id(* $1, @1); }
	| 	    PARSTART pattern COLON pattern PAREND           { $$ = new ListSplit(unique_ptr<Pattern>($2), unique_ptr<Pattern>($4), @2);  }
	| 	    PARSTART patterns_comma_ne COMMA pattern PAREND { $2->push_back(unique_ptr<Pattern>($4)); $$ = new TuplePattern(move(* $2), @2); delete $2; }
    |       SQSTART patterns_comma SQEND                    { $$ = new ListPattern(move(* $2), @2); delete $2; }
patterns_comma:    patterns_comma_ne                        { $$ = $1; }
	|	                                                    { $$ = new vector<unique_ptr<Pattern>>(); }
patterns_comma_ne:  patterns_comma_ne COMMA pattern         { $$ = $1; $$->push_back(unique_ptr<Pattern>($3)); }
	|   pattern                                             { $$ = new vector<unique_ptr<Pattern>>(); $$->push_back(unique_ptr<Pattern>($1)); }
literal:	INTLITERAL                                      { $$ = new IntExpression($1, @1); }
	|	FLOATLITERAL                                        { $$ = new FloatExpression($1, @1); }
	|	CHARLITERAL                                         { $$ = new CharExpression($1, @1); }
	|	STRINGLITERAL                                       { /* TODO STRINGLITERALS should become char lists $$ = new String(* $1, @1); */  }
expr:	expr OR expr                                        { $$ = new Or(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr AND expr                                       { $$ = new And(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr EQUAL expr                                     { $$ = new Equal(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr NOTEQUAL expr                                  { $$ = new NotEqual(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr LESSER expr                                    { $$ = new Lesser(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr GREATER expr                                   { $$ = new Greater(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr LESSEREQUAL expr                               { $$ = new LesserEq(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr GREATEREQUAL expr                              { $$ = new GreaterEq(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr ADD expr                                       { $$ = new Add(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr SUB expr                                       { $$ = new Sub(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr MUL expr                                       { $$ = new Mul(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr DIV expr                                       { $$ = new Div(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr MOD expr                                       { $$ = new Mod(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|   expr PROCON expr                                    { $$ = new ProducerConsumer(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|   expr CONCAT expr                                    { $$ = new Concat(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|	expr COLON expr                                     { $$ = new ListAdd(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @2); }
	|   expr TO type                                        { $$ = new To(unique_ptr<Expression>($1), @1); }
	|	ID                                                  { $$ = new Id(* $1, @1); }
	|	literal                                             { $$ = $1; }
	|	SQSTART exprs_comma SQEND                           { $$ = new ListExpression(move(* $2), @1); delete $2; }
	|   PARSTART exprs_comma_ne COMMA expr PAREND           { $2->push_back(unique_ptr<Expression>($4)); $$ = new TupleExpression(move(* $2), @1); delete $2; }
	|	PARSTART expr PAREND                                { $$ = new Par(unique_ptr<Expression>($2), @1); }
	|	expr PARSTART exprs_comma PAREND                    { $$ = new Call(unique_ptr<Expression>($1), move(* $3), @2); delete $3; }
	|	EXMARK expr                                         { $$ = new Not(unique_ptr<Expression>($2), @1); }
    |	SUB expr                                            { $$ = new Negative(unique_ptr<Expression>($2), @1); }
    |   BACKSLASH ids LAMBARROW expr                        { $$ = new LambdaFunction(unique_ptr<Expression>($4), move(* $2), @1); delete $2; }
ids:    ids ID                                              { $$->push_back(unique_ptr<IdPattern>(new IdPattern($2, @1))); }
    |                                                       { $$ = new vector<unique_ptr<IdPattern>>(); }
exprs_comma:    exprs_comma_ne                              { $$ = $1; }
	|	                                                    { $$ = new vector<unique_ptr<Expression>>(); }
exprs_comma_ne: exprs_comma_ne COMMA expr                   { $$ = $1; $$->push_back(unique_ptr<Expression>($3)); }
	|	        expr                                        { $$ = new vector<unique_ptr<Expression>>(); $$->push_back(unique_ptr<Expression>($1)); }

%%

void parser::Parser::error(const Parser::location_type& loc,
                           const std::string& msg) {
    Drv.error(loc, msg);
}

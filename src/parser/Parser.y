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
%token <val.long_int> INTLITERAL
%token <val.str> ID STRINGLITERAL
%token <val.chr> CHARLITERAL
%token <val.boolean> BOOLLITERAL
%token <val.long_double> FLOATLITERAL

%left OR
%left AND
%left EQUAL NOTEQUAL
%left LESSER GREATER GREATEREQUAL LESSEREQUAL
%left ADD SUB
%left MUL DIV MOD
%right COLON
%precedence EXMARK

//%type <valprog> program
%type <val.func_vec> funcs_ne
%type <val.func> func decl
%type <val.type_vec> types_comma_ne
%type <val.type> type signature
%type <val.cse_vec> cases_ne
%type <val.cse> case
%type <val.pattern_vec> patterns patterns_comma_ne patterns_comma
%type <val.pattern> pattern literal
%type <val.expr> expr struct_inst
%type <val.expr_vec> exprs_comma exprs_comma_ne

%{

#include "Driver.h"
#include "Scanner.h"

// Connect bison parser to flex scanner
#undef yylex
#define yylex driver.lexer->lex

#define move_uniq_vec(SRC, DIST) for (auto& i: SRC) DIST.push_back(move(i))

%}


%start program

%%

program:	funcs_ne                                        { driver.program = make_unique<Program>(move($1), @1); }
    |       expr                                            { driver.program = make_unique<Program>(move($1), @1); };
funcs_ne:	funcs_ne func                                   { $$ = move($1); $$.push_back(move($2)); }
	|       func                                            { $$ = vector<unique_ptr<Function>>(); $$.push_back(move($1)); } ;
func:		decl cases_ne                                   { $$ = move($1); move_uniq_vec($2, $$->cases); }
//func:		decl cases_ne                                   { $$ = move($1); $$->cases(move($2)) ; }
decl:		DEF ID COLON signature                          { $$ = make_unique<Function>(* $2, $4, @1); }
signature:	signature ARROR type                            { $$ = $1; $$.subtypes.push_back($3); }
	|       type                                            { $$ = Type(TypeId::SIGNATURE, @1); $$.subtypes.push_back($1); } ;
type:	BOOLTYPE                                            { $$ = Type(TypeId::BOOL, @1); }
	|	INTTYPE                                             { $$ = Type(TypeId::INT, @1); }
	|	FLOATTYPE                                           { $$ = Type(TypeId::FLOAT, @1); }
	|	CHARTYPE                                            { $$ = Type(TypeId::CHAR, @1); }
	|	STRINGTYPE                                          { $$ = Type(TypeId::STRING, @1); }
	|	SQSTART type SQEND                                  { $$ = Type(TypeId::LIST, @2); $$.subtypes.push_back($2); }
	|	PARSTART signature PAREND                           { $$ = $2; }
	|	PARSTART types_comma_ne COMMA type PAREND           { $$ = Type(TypeId::TUPLE, $2, @2); $$.subtypes.push_back($4); }
types_comma_ne: types_comma_ne COMMA type                   { $$ = move($1); $$.push_back($3); }
	|	type                                                { $$ = vector<Type>(); $$.push_back($1); };
cases_ne:	cases_ne case                                   { $$ = move($1); $$.push_back(move($2)); }
	|   case                                                { $$ = vector<unique_ptr<Case>>(); $$.push_back(move($1)); };
case: 		PIPE patterns ASSIGN expr                       { $$ = make_unique<Case>(move($4), move($2), @2); }
patterns:	patterns pattern                                { $$ = move($1); $$.push_back(move($2)); }
	|                                                       { $$ = vector<unique_ptr<Pattern>>(); } ;
pattern:    literal                                         { $$ = move($1); }
	| 	ID                                                  { $$ = make_unique<Id>(* $1, @1); }
	| 	PARSTART pattern COLON pattern PAREND               { $$ = make_unique<ListSplit>(move($2), move($4), @2);  }
	| 	PARSTART patterns_comma_ne COMMA pattern PAREND     { $2.push_back(move($4)); $$ = make_unique<TuplePattern>(move($2), @2);  }
    |   SQSTART patterns_comma SQEND                        { $$ = make_unique<ListPattern>(move($2), @2); }
patterns_comma:    patterns_comma_ne                        { $$ = move($1); }
	|	                                                    { $$ = vector<unique_ptr<Pattern>>(); } ;
patterns_comma_ne:  patterns_comma_ne COMMA pattern         { $$ = move($1); $$.push_back(move($3)); }
	|   pattern                                             { $$ = vector<unique_ptr<Pattern>>(); $$.push_back(move($1)); } ;
literal:	INTLITERAL                                      { $$ = make_unique<Int>($1, @1); }
	|	SUB INTLITERAL                                      { $$ = make_unique<Int>(- $2, @2); }
	|	FLOATLITERAL                                        { $$ = make_unique<Float>($1, @1); }
	|	SUB FLOATLITERAL                                    { $$ = make_unique<Float>(- $2, @2); }
	|	CHARLITERAL                                         { $$ = make_unique<Char>($1, @1); }
	|	STRINGLITERAL                                       { $$ = make_unique<String>(* $1, @1); }
	|	BOOLLITERAL                                         { $$ = make_unique<Bool>($1, @1); } ;
expr:	expr OR expr                                        { $$ = make_unique<Or>(move($1), move($3), @2); }
	|	expr AND expr                                       { $$ = make_unique<And>(move($1), move($3), @2); }
	|	expr EQUAL expr                                     { $$ = make_unique<Equal>(move($1), move($3), @2); }
	|	expr NOTEQUAL expr                                  { $$ = make_unique<NotEqual>(move($1), move($3), @2); }
	|	expr LESSER expr                                    { $$ = make_unique<Lesser>(move($1), move($3), @2); }
	|	expr GREATER expr                                   { $$ = make_unique<Greater>(move($1), move($3), @2); }
	|	expr LESSEREQUAL expr                               { $$ = make_unique<LesserEq>(move($1), move($3), @2); }
	|	expr GREATEREQUAL expr                              { $$ = make_unique<GreaterEq>(move($1), move($3), @2); }
	|	expr ADD expr                                       { $$ = make_unique<Add>(move($1), move($3), @2); }
	|	expr SUB expr                                       { $$ = make_unique<Sub>(move($1), move($3), @2); }
	|	expr MUL expr                                       { $$ = make_unique<Mul>(move($1), move($3), @2); }
	|	expr DIV expr                                       { $$ = make_unique<Div>(move($1), move($3), @2); }
	|	expr MOD expr                                       { $$ = make_unique<Mod>(move($1), move($3), @2); }
	|	expr COLON expr                                     { $$ = make_unique<ListAdd>(move($1), move($3), @2); }
	|	ID                                                  { $$ = make_unique<Id>(* $1, @1); }
	|	literal                                             { $$ = move($1); }
	|	struct_inst                                         { $$ = move($1); }
	|	PARSTART expr PAREND                                { $$ = make_unique<Par>(move($2), @1); }
	|	expr PARSTART exprs_comma PAREND                    { $$ = make_unique<Call>(move($1), move($3), @2); }
	|	EXMARK expr                                         { $$ = make_unique<Not>(move($2), @1); } ;
struct_inst:	SQSTART exprs_comma SQEND                   { $$ = make_unique<List>(move($2), @1); }
	|	PARSTART exprs_comma_ne COMMA expr PAREND           { $2.push_back(move($4)); $$ = make_unique<Tuple>(move($2), @1); }  ;
exprs_comma:    exprs_comma_ne                              { $$ = move($1); }
	|	                                                    { $$ = vector<unique_ptr<Expr>>(); } ;
exprs_comma_ne: exprs_comma_ne COMMA expr                   { $$ = move($1); $$.push_back(move($3));  }
	|	expr                                                { $$ = vector<unique_ptr<Expr>>(); $$.push_back(move($1)); } ;

%%

void parser::Parser::error(const Parser::location_type& loc,
                           const std::string& msg)
{
    driver.error(loc, msg);
}

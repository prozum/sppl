%name-prefix "parser"
%define "parser_class_name" {Parser}
%locations
%define api.location.type {common::Location}
%initial-action {
    @$.begin.Src = @$.end.Src = Drv.Source;
}
%parse-param { class Driver &Drv }
%debug
%error-verbose

// Parser.h
%code requires {
#include "Common.h"

#include <string>
#include <vector>

}

// Parser.cpp
%{
#include "Driver.h"

using namespace std;
using namespace common;

// Connect bison parser to flex scanner
#undef yylex
#define yylex Drv.Scr.lex
%}

%union {
    char                                Chr;
    long                                LongInt;
    long double                         LongDouble;
    bool                                Boolean;

    std::string *                       Str;

    common::Declaration *                       Decl;
    common::Function *                          Func;
    common::AlgebraicDT *                       ADT;
    common::Product *                           Prod;
    common::Type *                              Ty;
    common::Case *                              Cse;
    common::Pattern *                           Pat;
    common::Expression *                        Expr;

    std::vector<std::unique_ptr<common::Declaration>> *   DeclVec;
    std::vector<common::Type> *                           TypeVec;
    std::vector<std::unique_ptr<common::Case>> *          CaseVec;
    std::vector<std::unique_ptr<common::Pattern>> *       PatternVec;
    std::vector<std::unique_ptr<common::LambdaArg>> *     ArgVec;
    std::vector<std::unique_ptr<common::Expression>> *    ExprVec;
    std::vector<std::unique_ptr<common::Product>> *       ProdVec;
}

%token END 0 "End"
%token EOL "End of line"
%token DEF "def"
%token TYPE "type"
%token BACKSLASH "\\"
%token INCLUDE "include"
%token WHEN "when"
%token TO "to"
%token WILD "_"
%token CONCAT "++"
%token PROCON "|>"
%token INTTYPE "Int Type"
%token BOOLTYPE "Bool Type"
%token FLOATTYPE "Float Type"
%token STRINGTYPE "String Type"
%token VOIDTYPE "Void Type"
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
%token CURLSTART "{"
%token CURLEND "}"
%token EXMARK "!"
%token COMMA ","
%token PIPE "|"
%token COLONCOLON "::"
%token COLON ":"
%token <LongInt> INTLITERAL "Int"
%token <Chr> CHARLITERAL "Char"
%token <Boolean> BOOLLITERAL "Bool"
%token <LongDouble> FLOATLITERAL "Float"
%token <Str> IDSMALL IDBIG STRINGLITERAL "String"

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
%precedence EXMARK NEGATIVE

%type <DeclVec> decls
%type <Decl> decl include
%type <Func> func
%type <ADT> adt
%type <Prod> product
%type <TypeVec> generics types
%type <Ty> signature type tupletype generic
%type <Cse> case
%type <CaseVec> cases_ne
%type <PatternVec> patterns patterns_comma patterns_comma_ne
%type <Pat> pattern
%type <Expr> expr literal
%type <ArgVec> args
%type <ExprVec> exprs_comma exprs_comma_ne exprs
%type <ProdVec> sum

// Start at program
%start program

%%

program:	decls                                           { Drv.Prog = make_unique<Program>(move(* $1), @1); delete $1; }
    |       expr                                            { Drv.Prog = make_unique<Program>(unique_ptr<Expression>($1), @1); }
decls:      decls decl                                      { $$ = $1; if ($2) $$->push_back(unique_ptr<Declaration>($2)); }
    |                                                       { $$ = new vector<unique_ptr<Declaration>>(); }
decl:       func                                            { $$ = $1; }
    |       adt                                             { $$ = $1; }
    |       include                                         { $$ = $1; }
include:    INCLUDE STRINGLITERAL                           { $$ = nullptr; Drv.Files.push_back(* $2); }
func:		DEF IDSMALL COLON signature cases_ne            { $$ = new Function(* $2, * $4, @1); $$->Cases = move(* $5); delete $2; }
adt:        TYPE IDBIG generics ASSIGN sum                  { $$ = new AlgebraicDT(* $2, move(* $3), move(* $5), @1); delete $2; }
sum:        sum PIPE product                                { $$ = $1; $$->push_back(unique_ptr<Product>($3)); }
    |       product                                         { $$ = new vector<unique_ptr<Product>>(); $$->push_back(unique_ptr<Product>($1)); }
product:    IDBIG types                                     { $$ = new Product(* $1, * $2, @1); delete $1; delete $2; }
types:      types type                                      { $$ = $1; $$->push_back(* $2); delete $2; }
    |                                                       { $$ = new vector<Type>(); }
generics:   generics generic                                { $$ = $1; $$->push_back(* $2); delete $2; }
    |                                                       { $$ = new vector<Type>(); }
signature:	signature ARROR type                            { $$ = $1; $$->Subtypes.push_back(* $3); }
	|       type                                            { $$ = new Type(TypeId::SIGNATURE, @1); $$->Subtypes.push_back(* $1); }
type:	    INTTYPE                                         { $$ = new Type(TypeId::INT, @1); }
	|	    FLOATTYPE                                       { $$ = new Type(TypeId::FLOAT, @1); }
	|	    CHARTYPE                                        { $$ = new Type(TypeId::CHAR, @1); }
	|	    STRINGTYPE                                      { $$ = new Type(TypeId::STRING, vector<Type>({ Type(TypeId::CHAR) }), @1); }
	|	    BOOLTYPE                                        { $$ = new Type(TypeId::BOOL, @1); }
    |	    VOIDTYPE                                        { $$ = new Type(TypeId::VOID, @1); }
	|	    SQSTART type SQEND                              { $$ = new Type(TypeId::LIST, @1); $$->Subtypes.push_back(* $2); }
	|	    PARSTART signature PAREND                       { $$ = $2; }
	|	    PARSTART tupletype COMMA type PAREND            { $$ = $2; $$->Loc = @1; $$->Subtypes.push_back(* $4); }
    |       generic                                         { $$ = $1; }
	|       IDBIG types                                     { $$ = new Type(TypeId::CUSTOM, * $2, @1); $$->Name = * $1; delete $1; delete $2; }
generic:    IDSMALL                                         { $$ = new Type(TypeId::GENERIC, @1); $$->Name = * $1; delete $1; }
tupletype:  tupletype COMMA type                            { $$ = $1; $$->Subtypes.push_back(* $3); }
	|	    type                                            { $$ = new Type(TypeId::TUPLE); $$->Subtypes.push_back(* $1); }
cases_ne:	cases_ne case                                   { $$ = $1; $$->push_back(unique_ptr<Case>($2)); }
	|       case                                            { $$ = new vector<unique_ptr<Case>>(); $$->push_back(unique_ptr<Case>($1)); }
case: 		PIPE patterns ASSIGN expr                       { $$ = new Case(unique_ptr<Expression>($4), nullptr, move(* $2), @1); delete $2; }
    |		PIPE patterns WHEN expr ASSIGN expr             { $$ = new Case(unique_ptr<Expression>($6), unique_ptr<Expression>($4), move(* $2), @1); delete $2; }
patterns:	patterns pattern                                { $$ = $1; $$->push_back(unique_ptr<Pattern>($2)); }
	|                                                       { $$ = new vector<unique_ptr<Pattern>>(); }
pattern:    INTLITERAL                                      { $$ = new IntPattern($1, @1); }
    |	    SUB INTLITERAL                                  { $$ = new IntPattern(- $2, @1); }
    |	    FLOATLITERAL                                    { $$ = new FloatPattern($1, @1); }
    |	    SUB FLOATLITERAL                                { $$ = new FloatPattern(- $2, @1); }
	|	    CHARLITERAL                                     { $$ = new CharPattern($1, @1); }
	|	    BOOLLITERAL                                     { $$ = new BoolPattern($1, @1); }
	|	    STRINGLITERAL                                   { $$ = new StringPattern(* $1, @1); delete $1; }
	| 	    IDSMALL                                         { $$ = new IdPattern(* $1, @1); }
	| 	    pattern COLON pattern                           { $$ = new ListSplit(unique_ptr<Pattern>($1), unique_ptr<Pattern>($3), @1);  }
	| 	    IDBIG patterns                                  { $$ = new AlgebraicPattern(* $1, move(* $2), @1); delete $1; delete $2; }
	| 	    PARSTART patterns_comma_ne COMMA pattern PAREND { $2->push_back(unique_ptr<Pattern>($4)); $$ = new TuplePattern(move(* $2), @1); delete $2; }
    |       SQSTART patterns_comma SQEND                    { $$ = new ListPattern(move(* $2), @1); delete $2; }
    |       PARSTART pattern PAREND                         { $$ = new ParPattern(unique_ptr<Pattern>($2), @1); }
    |       WILD                                            { $$ = new WildPattern(@1); }
patterns_comma:    patterns_comma_ne                        { $$ = $1; }
	|	                                                    { $$ = new vector<unique_ptr<Pattern>>(); }
patterns_comma_ne:  patterns_comma_ne COMMA pattern         { $$ = $1; $$->push_back(unique_ptr<Pattern>($3)); }
	|   pattern                                             { $$ = new vector<unique_ptr<Pattern>>(); $$->push_back(unique_ptr<Pattern>($1)); }
literal:	INTLITERAL                                      { $$ = new IntExpr($1, @1); }
	|	FLOATLITERAL                                        { $$ = new FloatExpr($1, @1); }
	|	CHARLITERAL                                         { $$ = new CharExpr($1, @1); }
	|	BOOLLITERAL                                         { $$ = new BoolExpr($1, @1); }
	|	STRINGLITERAL                                       { $$ = new StringExpr(* $1, @1); delete $1; }
expr:	expr OR expr                                        { $$ = new Or(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr AND expr                                       { $$ = new And(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr EQUAL expr                                     { $$ = new Equal(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr NOTEQUAL expr                                  { $$ = new NotEqual(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr LESSER expr                                    { $$ = new Lesser(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr GREATER expr                                   { $$ = new Greater(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr LESSEREQUAL expr                               { $$ = new LesserEq(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr GREATEREQUAL expr                              { $$ = new GreaterEq(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr ADD expr                                       { $$ = new Add(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr SUB expr                                       { $$ = new Sub(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr MUL expr                                       { $$ = new Mul(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr DIV expr                                       { $$ = new Div(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr MOD expr                                       { $$ = new Mod(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|   expr PROCON expr                                    { $$ = new ProducerConsumer(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|   expr CONCAT expr                                    { $$ = new Concat(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|	expr COLON expr                                     { $$ = new ListAdd(unique_ptr<Expression>($1), unique_ptr<Expression>($3), @1); }
	|   expr TO type                                        { $$ = new To(unique_ptr<Expression>($1), * $3, @1); delete $3; }
	|	IDSMALL                                             { $$ = new IdExpr(* $1, @1); }
	|	literal                                             { $$ = $1; }
	|	SQSTART exprs_comma SQEND                           { $$ = new ListExpr(move(* $2), @1); /* delete $2; */ }
	| 	IDBIG exprs                                         { $$ = new AlgebraicExpr(* $1, move(* $2), @1); delete $1; delete $2; }
	|   PARSTART exprs_comma_ne COMMA expr PAREND           { $2->push_back(unique_ptr<Expression>($4)); $$ = new TupleExpr(move(* $2), @1); delete $2; }
	|	PARSTART expr PAREND                                { $$ = new ParExpr(unique_ptr<Expression>($2), @1); }
	|	expr PARSTART exprs_comma PAREND                    { $$ = new CallExpr(unique_ptr<Expression>($1), move(* $3), @1); delete $3; }
    |   BACKSLASH args LAMBARROW expr                       { $$ = new LambdaFunction(unique_ptr<Expression>($4), move(* $2), @1); delete $2; }
    |   EXMARK expr                                         { $$ = new Not(unique_ptr<Expression>($2), @1); }
    |	SUB expr %prec NEGATIVE                             { $$ = new Negative(unique_ptr<Expression>($2), @1); }
args:    args IDSMALL                                       { $$ = $1; $$->push_back(unique_ptr<LambdaArg>(new LambdaArg(* $2, @1))); }
    |                                                       { $$ = new vector<unique_ptr<LambdaArg>>();  }
exprs: exprs expr                                           { $$ = $1; $$->push_back(unique_ptr<Expression>($2)); }
	|                                                       { $$ = new vector<unique_ptr<Expression>>(); }
exprs_comma:    exprs_comma_ne                              { $$ = $1; }
	|	                                                    { $$ = new vector<unique_ptr<Expression>>(); }
exprs_comma_ne: exprs_comma_ne COMMA expr                   { $$ = $1; $$->push_back(unique_ptr<Expression>($3)); }
	|	        expr                                        { $$ = new vector<unique_ptr<Expression>>(); $$->push_back(unique_ptr<Expression>($1)); }

%%

void parser::Parser::error(const Parser::location_type& loc,
                           const std::string& msg) {
    Drv.error(loc, msg);
}

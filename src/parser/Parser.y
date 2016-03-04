%skeleton "lalr1.cc"
%name-prefix "parser"
%define "parser_class_name" {Parser}
%locations
%debug
%parse-param { class Driver& driver }

%{
#include <string>
#include <vector>

#include "Node.h"

%}

%union {
    common::Program *                  prog_val;
    common::Function *                 func_val;
    common::Type *                     type_val;
    common::Case *                     case_val;
    common::Pattern *                  pattern_val;
    common::Expr *                     expr_val;

    std::vector<common::Function *> *  func_vec;
    std::vector<common::Type *> *      type_vec;
    std::vector<common::Case *> *      case_vec;
    std::vector<common::Pattern *> *   pattern_vec;
    std::vector<common::Expr *> *      expr_vec;

    std::string*	                str_val;
    long	                        long_val;
    long double	                    longdouble_val;
    bool	                        bool_val;
}

%token T_END 0 "end of file"
%token T_DEF T_INTTYPE T_BOOLTYPE T_FLOATTYPE T_STRINGTYPE T_CHARTYPE T_ARROR T_EQUAL T_NOTEQUAL T_AND T_OR T_LESSEREQUAL T_GREATEREQUAL T_LESSER T_GREATER T_MUL T_DIV T_MOD T_ADD T_SUB T_ASSIGN T_SQSTART T_SQEND T_PARSTART T_PAREND T_EXMARK T_COMMA T_PIPE T_ARROW T_COLON
%token <long_val> T_INTLITERAL
%token <str_val> T_ID T_STRINGLITERAL T_CHARLITERAL
%token <bool_val> T_BOOLLITERAL
%token <longdouble_val> T_FLOATLITERAL

%left T_OR
%left T_AND
%left T_EQUAL T_NOTEQUAL
%left T_LESSER T_GREATER T_GREATEREQUAL T_LESSEREQUAL
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
%right T_COLON
%precedence T_EXMARK

%type <prog_val> program
%type <func_vec> funcs_ne
%type <func_val> func decl
%type <type_val> type signature
%type <type_vec> types_comma_ne
%type <case_vec> cases_ne
%type <case_val> case
%type <pattern_vec> patterns patterns_comma_ne
%type <pattern_val> pattern literal
%type <expr_val> expr struct_inst
%type <expr_vec> exprs_comma exprs_comma_ne

%{

#include "Driver.h"
#include "Scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

using namespace common;
using namespace std;

%}

%start program

%%

program:	funcs_ne                                        { driver.main = new Program(); driver.main->funcs = * $1; delete $1; };
funcs_ne:	funcs_ne func                                   { $$ = $1; $$->push_back($2); }
	| func                                                  { $$ = new std::vector<Function *>(); $$->push_back($1); } ;
func:		decl cases_ne                                   { $$ = $1; $$->cases = * $2; delete $2; }
decl:		T_DEF T_ID T_COLON signature                    { $$ = new Function(* $2); $$->types = $4->types; delete $2; delete $4; }
signature:	signature T_ARROR type                          { $$ = $1; $$->types.push_back($3); }
	| type                                                  { $$ = new Type(SIGNATURE); $$->types.push_back($1); } ;
type:		T_BOOLTYPE                                      { $$ = new Type(BOOL); }
	|	T_INTTYPE                                           { $$ = new Type(INT); }
	|	T_FLOATTYPE                                         { $$ = new Type(FLOAT); }
	|	T_CHARTYPE                                          { $$ = new Type(CHAR); }
	|	T_STRINGTYPE                                        { $$ = new Type(STRING); }
	|	T_SQSTART type T_SQEND                              { $$ = new Type(LIST); $$->types.push_back($2); }
	|	T_PARSTART signature T_PAREND                       { $$ = $2; }
	|	T_PARSTART types_comma_ne T_COMMA type T_PAREND     { $$ = new Type(TUPLE, $2); delete $2; $$->types.push_back($4); }
types_comma_ne: types_comma_ne T_COMMA type                 { $$ = $1; $$->push_back($3); }
	|	type                                                { $$ = new vector<Type *>(); $$->push_back($1); };
cases_ne:	cases_ne case                                   { $$ = $1; $$->push_back($2); }
	| case                                                  { $$ = new vector<Case *>(); $$->push_back($1); };
case: 		T_PIPE patterns T_ASSIGN expr                   { $$ = new Case($4); $$->patterns = * $2; delete $2; }
patterns:	patterns pattern                                { $$ = $1; $$->push_back($2); }
	|                                                       { $$ = new vector<Pattern *>(); } ;
pattern:	literal                                         { $$ = $1; }
	| 	T_ID                                                { $$ = new Id(* $1); delete $1;  }
	| 	T_PARSTART pattern T_COLON pattern T_PAREND         { $$ = new ListSplit($2, $4);  }
	| 	T_PARSTART patterns_comma_ne T_COMMA pattern T_PAREND { TuplePattern *t = new TuplePattern(); t->patterns = * $2; delete $2; t->patterns.push_back($4); $$ = t; }
patterns_comma_ne: patterns_comma_ne T_COMMA pattern        { $$ = $1; $$->push_back($3); }
	| pattern                                               { $$ = new vector<Pattern *>(); $$->push_back($1); } ;
literal:	T_INTLITERAL                                    { $$ = new Int($1); }
	|	T_SUB T_INTLITERAL                                  { $$ = new Int(- $2); }
	|	T_FLOATLITERAL                                      { $$ = new Float($1); }
	|	T_SUB T_FLOATLITERAL                                { $$ = new Float(- $2); }
	|	T_CHARLITERAL                                       { $$ = new Char(* $1); delete $1; }
	|	T_STRINGLITERAL                                     { $$ = new String(* $1); delete $1; }
	|	T_BOOLLITERAL                                       { $$ = new Bool($1); } ;
expr:	expr T_OR expr                                      { $$ = new Or($1, $3); }
	|	expr T_AND expr                                     { $$ = new And($1, $3); }
	|	expr T_EQUAL expr                                   { $$ = new Equal($1, $3); }
	|	expr T_NOTEQUAL expr                                { $$ = new NotEqual($1, $3); }
	|	expr T_LESSER expr                                  { $$ = new Lesser($1, $3); }
	|	expr T_GREATER expr                                 { $$ = new Greater($1, $3); }
	|	expr T_LESSEREQUAL expr                             { $$ = new LesserEq($1, $3); }
	|	expr T_GREATEREQUAL expr                            { $$ = new GreaterEq($1, $3); }
	|	expr T_ADD expr                                     { $$ = new Add($1, $3); }
	|	expr T_SUB expr                                     { $$ = new Sub($1, $3); }
	|	expr T_MUL expr                                     { $$ = new Mul($1, $3); }
	|	expr T_DIV expr                                     { $$ = new Div($1, $3); }
	|	expr T_MOD expr                                     { $$ = new Mod($1, $3); }
	|	expr T_COLON expr                                   { $$ = new ListAdd($1, $3); }
	|	T_ID                                                { $$ = new Id(* $1); delete $1; }
	|	literal                                             { $$ = $1; }
	|	struct_inst                                         { $$ = $1; }
	|	T_PARSTART expr T_PAREND                            { $$ = new Par($2); }
	|	expr T_PARSTART exprs_comma T_PAREND                { Call *t = new Call($1); t->exprs = * $3; delete $3; $$ = t; }
	|	T_EXMARK expr                                       { $$ = new Not($2); } ;
struct_inst:	T_SQSTART exprs_comma T_SQEND               { List *t = new List(); t->exprs = * $2; delete $2; $$ = t; }
	|	T_PARSTART exprs_comma_ne T_COMMA expr T_PAREND     { Tuple *t = new Tuple(); t->exprs = * $2; delete $2; t->exprs.push_back($4); $$ = t; }  ;
exprs_comma:	exprs_comma_ne                              { $$ = $1; }
	|	                                                    { $$ = new vector<Expr *>(); } ;
exprs_comma_ne:	exprs_comma_ne T_COMMA expr                 { $$ = $1; $$->push_back($3);  }
	|	expr                                                { $$ = new vector<Expr *>(); $$->push_back($1); } ;

%%

void parser::Parser::error(const Parser::location_type& l,
                            const std::string& m)
{
    driver.error(l, m);
}

%{
    #include <string>
    #include <map>
    using namespace std;
    map<string, double> vars;
    extern int yylex();
    extern void yyerror(char*);
%}

%union {
    long	long_val;
    long double	longdouble_val;
    bool	bool_val;
    string *	str_val;
}

%token T_DEF T_INTTYPE T_BOOLTYPE T_FLOATTYPE T_STRINGTYPE T_CHARTYPE T_ARROR T_EQUAL T_NOTEQUAL T_AND T_OR T_LESSEREQUAL T_GREATEREQUAL T_LESSER T_GREATER T_MUL T_DIV T_MOD T_ADD T_SUB T_ASSIGN T_SQSTART T_SQEND T_PARSTART T_PAREND T_EXMARK T_COMMA T_PIPE T_ARROW T_COLON
%token <long_val> T_INTLITERAL
%token <str_val> T_ID T_STRINGLITERAL T_CHARLITERAL T_FLOATLITERAL
%token <bool_val> T_BOOLLITERAL

%left T_OR
%left T_AND
%left T_EQUAL T_NOTEQUAL
%left T_LESSER T_GREATER T_GREATEREQUAL T_LESSEREQUAL
%left T_ADD T_SUB 
%left T_MUL T_DIV T_MOD 
%right T_COLON
%precedence T_EXMARK

%start program

%%

program:	funcs_ne ;
funcs_ne:	funcs_ne func | func ;
func:		decl cases_ne ;
decl:		T_DEF T_ID T_COLON signature ;
signature:	type T_ARROR signature | type ;
type:		T_BOOLTYPE
	|	T_INTTYPE
	|	T_FLOATTYPE
	|	T_CHARTYPE
	|	T_STRINGTYPE
	|	T_SQSTART type T_SQEND
	|	T_PARSTART signature T_PAREND
	|	T_PARSTART type T_COMMA types_comma_ne T_PAREND ;
types_comma_ne: type T_COMMA types_comma_ne
	|	type ;
cases_ne:	case cases_ne | case ;
case: 		T_PIPE patterns T_ASSIGN expr ;
patterns:	pattern patterns |  ;
pattern:	literal 
	| 	T_ID 
	| 	T_PARSTART pattern T_COLON pattern T_PAREND 
	| 	T_PARSTART pattern T_COMMA patterns_comma_ne T_PAREND 
	|	T_SQSTART patterns_comma T_SQEND ;
patterns_comma: patterns_comma_ne | ;
patterns_comma_ne: pattern patterns_comma_ne | pattern ;
literal:	T_INTLITERAL
	|	T_SUB T_INTLITERAL
	|	T_FLOATLITERAL
	|	T_SUB T_FLOATLITERAL
	|	T_CHARLITERAL
	|	T_STRINGLITERAL
	|	T_BOOLLITERAL ;
expr:		expr T_OR expr
	|	expr T_AND expr
	|	expr T_EQUAL expr
	|	expr T_NOTEQUAL expr
	|	expr T_LESSER expr
	|	expr T_GREATER expr
	|	expr T_LESSEREQUAL expr
	|	expr T_GREATEREQUAL expr
	|	expr T_ADD expr
	|	expr T_SUB expr
	|	expr T_MUL expr
	|	expr T_DIV expr
	|	expr T_MOD expr
	|	expr T_COLON expr
	|	T_ID
	|	literal
	|	struct_inst
	|	T_PARSTART expr T_PAREND
	|	expr T_PARSTART expr T_PAREND
	|	T_EXMARK expr ;
struct_inst:	T_SQSTART exprs_comma T_SQEND
	|	T_PARSTART expr T_COMMA exprs_comma_ne T_PAREND ;
exprs_comma:	exprs_comma_ne
	|	;
exprs_comma_ne:	expr T_COMMA exprs_comma_ne
	|	expr ;

%%



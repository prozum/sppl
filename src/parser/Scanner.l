%option c++
%option noyywrap
%option prefix="Sppl"

%{

#include <string>

#include "Node.h";
#include "Scanner.h"

typedef parser::Parser::token token;
typedef parser::Parser::token_type token_type;

#define yyterminate() return token::T_END

using namespace std;
%}


%%
[ \n\t]				{  }
"#".*\n				{  }
"def"				{ return token::T_DEF; }
"Int"				{ return token::T_INTTYPE; }
"Bool"				{ return token::T_BOOLTYPE; }
"Float"				{ return token::T_FLOATTYPE; }
"String"			{ return token::T_STRINGTYPE; }
"Char"				{ return token::T_CHARTYPE; }
[0-9]+				{ yylval->long_val = atol(yytext); return token::T_INTLITERAL; }
[0-9]+"."[0-9]+			{ yylval->longdouble_val = atof(yytext); return token::T_FLOATLITERAL; }
"'".+"'"			{ yylval->str_val = new string(yytext); return token::T_CHARLITERAL; }
"\"".*"\""			{ yylval->str_val = new string(yytext); return token::T_STRINGLITERAL; }
"True"|"False"			{ yylval->bool_val = (string(yytext) == "True")? true : false; return token::T_BOOLLITERAL; }
[a-zA-Z][a-zA-Z0-9]*		{ yylval->str_val = new string(yytext); return token::T_ID; }
"->"				{ return token::T_ARROR; }
"=="				{ return token::T_EQUAL; }
"!="				{ return token::T_NOTEQUAL; }
"&&"				{ return token::T_AND; }
"||"				{ return token::T_OR; }
"<="				{ return token::T_LESSEREQUAL; }
">="				{ return token::T_GREATEREQUAL; }
"<"				{ return token::T_LESSER; }
">"				{ return token::T_GREATER; }
"*"				{ return token::T_MUL; }
"/"				{ return token::T_DIV; }
"%"				{ return token::T_MOD; }
"+"				{ return token::T_ADD; }
"-"				{ return token::T_SUB; }
"="				{ return token::T_ASSIGN; }
"["				{ return token::T_SQSTART; }
"]"				{ return token::T_SQEND; }
"("				{ return token::T_PARSTART; }
")"				{ return token::T_PAREND; }
"!"				{ return token::T_EXMARK; }
","				{ return token::T_COMMA; }
":"				{ return token::T_COLON; }
"|"				{ return token::T_PIPE; }

%%

namespace parser {

Scanner::Scanner(std::istream* in,
                 std::ostream* out)
    : SpplFlexLexer(in, out)
{
}

Scanner::~Scanner()
{
}

void Scanner::set_debug(bool b)
{
    yy_flex_debug = b;
}

}

#ifdef yylex
#undef yylex
#endif

int SpplFlexLexer::yylex()
{
    std::cerr << "in SpplFlexLexer::yylex() !" << std::endl;
    return 0;
}


%option c++
%option noyywrap
%option prefix="Sppl"

%{

#include <string>

#include "Node.h"
#include "Scanner.h"

#define YY_NO_UNISTD_H

// Ignore register storage class since it is deprecated in C++11
#define register

typedef parser::Parser::token token;
typedef parser::Parser::token_type token_type;

#define yyterminate() return token::END
#define YY_USER_ACTION  yylloc->columns(yyleng);

using namespace std;
%}


%%

%{
    yylloc->step();
%}

[ \t] {  }

[\n] {
    yylloc->lines(yyleng);
    yylloc->step();
}

"#".*\n	{ }

"def" {
    return token::DEF;
}

"Int" {
    return token::INTTYPE;
}

"Bool"	{
    return token::BOOLTYPE;
}

"Float"	{
    return token::FLOATTYPE;
}

"String" {
    return token::STRINGTYPE;
}

"Char"	{
return token::CHARTYPE;
}

[0-9]+ {
    yylval->long_val = atol(yytext);
    return token::INTLITERAL;
}

[0-9]+"."[0-9]+ {
    yylval->longdouble_val = atof(yytext);
    return token::FLOATLITERAL;
}

"'".+"'" {
    yylval->str_val = new string(yytext);
    return token::CHARLITERAL;
}

"\"".*"\"" {
    yylval->str_val = new string(yytext);
    return token::STRINGLITERAL;
}

"True"|"False" {
    yylval->bool_val = (string(yytext) == "True") ? true : false;
    return token::BOOLLITERAL;
}

[a-zA-Z][a-zA-Z0-9]* {
    yylval->str_val = new string(yytext);
    return token::ID;
}

"->" {
    return token::ARROR;
}

"==" {
    return token::EQUAL;
}

"!=" {
    return token::NOTEQUAL;
}

"&&" {
    return token::AND;
}

"||" {
    return token::OR;
}
"<=" {
    return token::LESSEREQUAL;
}
">=" {
    return token::GREATEREQUAL;
}

"<"	{
    return token::LESSER;
}

">" {
    return token::GREATER;
}

"*" {
    return token::MUL;
}

"/"	{
    return token::DIV;
}

"%"	{
    return token::MOD;
}

"+" {
    return token::ADD;
}

"-" {
    return token::SUB;
}

"=" {
    return token::ASSIGN;
}

"["	{
    return token::SQSTART;
}

"]"	{
    return token::SQEND;
}

"("	{
    return token::PARSTART;
}

")"	{
    return token::PAREND;
}

"!"	{
    return token::EXMARK;
}

","	{
    return token::COMMA;
}

":"	{
    return token::COLON;
}

"|"	{
    return token::PIPE;
}

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


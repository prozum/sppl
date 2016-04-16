%option c++
%option prefix="Sppl"

%{

#include <string>

#include "Node.h"
#include "Scanner.h"
#include "Driver.h"

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
    if (new_source) {
        yylloc->sources(driver.source);
        new_source = false;
    }
    yylloc->step();
%}

[ \t]+ {
    yylloc->step();
}

[\n]+ {
    yylloc->lines(yyleng);
    yylloc->step();
}

"#"[^\n]* {
    yylloc->lines(yyleng);
    yylloc->step();
}

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
    yylval->long_int = atol(yytext);
    return token::INTLITERAL;
}

[0-9]+"."[0-9]+ {
    yylval->long_double = atof(yytext);
    return token::FLOATLITERAL;
}

"'"[^']"'" {

    yylval->chr = yytext[1];
    return token::CHARLITERAL;
}

"\""[^"]*"\"" {
    yylval->str = new string(yytext);
    yylval->str->pop_back();
    yylval->str->erase (0,1);
    return token::STRINGLITERAL;
}

"True"|"False" {
    yylval->boolean = (string(yytext) == "True") ? true : false;
    return token::BOOLLITERAL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval->str = new string(yytext);
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

#include "Driver.h"

namespace parser {

    Scanner::Scanner(Driver &driver) :
        SpplFlexLexer(),
        driver(driver) { }

    Scanner::~Scanner() { }

    void Scanner::set_debug(bool b) {
        yy_flex_debug = b;
    }

    int Scanner::yywrap() {
        if (driver.next_input()) {
            new_source = true;
            return 0;
        }

        return 1;
    }
}

#ifdef yylex
#undef yylex
#endif

int SpplFlexLexer::yylex() {
    std::cerr << "in SpplFlexLexer::yylex() !" << std::endl;
    return 0;
}

int SpplFlexLexer::yywrap() {
    std::cerr << "in SpplFlexLexer::yywrap() !" << std::endl;
    return 0;
}



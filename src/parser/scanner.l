%option c++
%option prefix="Sppl"

%{

#include <string>

#include "Node.h"
#include "Scanner.h"
#include "Driver.h"

#define YY_NO_UNISTD_H

typedef parser::Parser::token token;
typedef parser::Parser::token_type token_type;

#define yyterminate() return token::END
#define YY_USER_ACTION  yylloc->columns(yyleng);

using namespace std;
%}


%%

%{
    if (NewSource) {
        yylloc->sources(Drv.Source);
        NewSource = false;
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

"type" {
    return token::TYPE;
}

"Int" {
    return token::INTTYPE;
}

"Float"	{
    return token::FLOATTYPE;
}

"Char"	{
    return token::CHARTYPE;
}

"Include" {
    return token::INCLUDE;
}

"to" {
    return token::TO;
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
    yylval->str->erase(0, 1);
    return token::STRINGLITERAL;
}

[a-zA-Z][a-zA-Z0-9_]* {
    yylval->str = new string(yytext);
    return token::ID;
}

"\\" {
    return token::BACKSLASH;
}

"->" {
    return token::ARROR;
}

"=>" {
    return token::LAMBARROR;
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

"_" {
    return token::WILD;
}

"++" {
    return token::CONCAT;
}

"|>" {
    return token::PROCON;
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

    Scanner::Scanner(Driver &Drv) :
        SpplFlexLexer(),
        Drv(Drv) { }

    Scanner::~Scanner() { }

    void Scanner::set_debug(bool b) {
        yy_flex_debug = b;
    }

    int Scanner::yywrap() {
        if (Drv.nextInput()) {
            NewSource = true;
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



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

"True" {
    yylval->Boolean = true;
    return token::BOOLLITERAL;
}

"False" {
    yylval->Boolean = false;
    return token::BOOLLITERAL;
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

"String" {
    return token::STRINGTYPE;
}

"Void"	{
    return token::VOIDTYPE;
}

"Bool"	{
    return token::BOOLTYPE;
}

"include" {
    return token::INCLUDE;
}

"to" {
    return token::TO;
}

"when" {
    return token::WHEN;
}

[0-9]+ {
    yylval->LongInt = atol(yytext);
    return token::INTLITERAL;
}

[0-9]+"."[0-9]+ {
    yylval->LongDouble = atof(yytext);
    return token::FLOATLITERAL;
}

"'"[^']"'" {
    yylval->Chr = yytext[1];
    return token::CHARLITERAL;
}

"\""[^"]*"\"" {
    yylval->Str = new string(yytext);
    yylval->Str->pop_back();
    yylval->Str->erase(0, 1);
    return token::STRINGLITERAL;
}

[a-z][a-zA-Z0-9_]* {
    yylval->Str = new string(yytext);
    return token::IDSMALL;
}

[A-Z][a-zA-Z0-9_]* {
    yylval->Str = new string(yytext);
    return token::IDBIG;
}

"\\" {
    return token::BACKSLASH;
}

"->" {
    return token::ARROR;
}

"=>" {
    return token::LAMBARROW;
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

"{"	{
    return token::CURLSTART;
}

"}"	{
    return token::CURLEND;
}

"!"	{
    return token::EXMARK;
}

","	{
    return token::COMMA;
}

"::"	{
    return token::COLONCOLON;
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



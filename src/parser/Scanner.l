%option c++
%option noyywrap

%{
    #include <cstdio>
    #include <iostream>
    #include <string>
    using namespace std;
    #include <vector>
    #include "Node.h"
    using namespace ast;
    #include "Parser.hpp"
    extern void yyerror(char*);
    int yyparse();
%}

%%
[ \n\t]				{  }
"#".*\n				{  }
"def"				{ return T_DEF; }
"Int"				{ return T_INTTYPE; }
"Bool"				{ return T_BOOLTYPE; }
"Float"				{ return T_FLOATTYPE; }
"String"			{ return T_STRINGTYPE; }
"Char"				{ return T_CHARTYPE; }
[0-9]+				{ yylval.long_val = atol(yytext); return T_INTLITERAL; }
[0-9]+"."[0-9]+			{ yylval.longdouble_val = atof(yytext); return T_FLOATLITERAL; }
"'".+"'"			{ yylval.str_val = new string(yytext); return T_CHARLITERAL; }
"\"".*"\""			{ yylval.str_val = new string(yytext); return T_STRINGLITERAL; }
"True"|"False"			{ yylval.bool_val = (string(yytext) == "True")? true : false; return T_BOOLLITERAL; }
[a-zA-Z][a-zA-Z0-9]*		{ yylval.str_val = new string(yytext); return T_ID; }
"->"				{ return T_ARROR; }
"=="				{ return T_EQUAL; }
"!="				{ return T_NOTEQUAL; }
"&&"				{ return T_AND; }
"||"				{ return T_OR; }
"<="				{ return T_LESSEREQUAL; }
">="				{ return T_GREATEREQUAL; }
"<"				{ return T_LESSER; }
">"				{ return T_GREATER; }
"*"				{ return T_MUL; }
"/"				{ return T_DIV; }
"%"				{ return T_MOD; }
"+"				{ return T_ADD; }
"-"				{ return T_SUB; }
"="				{ return T_ASSIGN; }
"["				{ return T_SQSTART; }
"]"				{ return T_SQEND; }
"("				{ return T_PARSTART; }
")"				{ return T_PAREND; }
"!"				{ return T_EXMARK; }
","				{ return T_COMMA; }
":"				{ return T_COLON; }
"|"				{ return T_PIPE; }

%%

%option c++
%option   warn nodefault
%option   yylineno
%option   outfile="scanner.c" header-file="scanner.h"
%option default

DIGIT    [0-9]
ID       [a-z][a-z0-9]*
     
%%


"def"					{ printf( "Def found: %s\n", yytext); }
{ID}					{ printf( "Id found: %s\n", yytext); }
":"						{ printf( "Colon found: %s\n", yytext); }
"->"					{ printf( "Arrow found: %s\n", yytext); }
"|"						{ printf( "Pipe found: %s\n", yytext); }
"="						{ printf( "Assign found: %s\n", yytext); }
"("						{ printf( "Parstart found: %s\n", yytext); }
")"						{ printf( "Parend found: %s\n", yytext); }
"Int"					{ printf( "Int found: %s\n", yytext); }
"Float"					{ printf( "Float found: %s\n", yytext); }
"Bool"					{ printf( "Bool found: %s\n", yytext); }
"Char"					{ printf( "Char found: %s\n", yytext); }
"String"				{ printf( "String found: %s\n", yytext); }
"["						{ printf( "Sqstart found: %s\n", yytext); }
"]"						{ printf( "Sqend found: %s\n", yytext); }
{DIGIT}+				{ printf( "Int Literal found: %s\n", yytext); }
{DIGIT}+"."{DIGIT}+		{ printf( "Float Literal found: %s\n", yytext); }
"True" | "False"		{ printf( "Bool Literal found: %s\n", yytext); }
\"[^']\"				{ printf( "Char Literal found: %s\n", yytext); }
\'[^"]\'				{ printf( "String Literal found: %s\n", yytext); }
"||"					{ printf( "Or found: %s\n", yytext); }
"&&"					{ printf( "And found: %s\n", yytext); }
"=="					{ printf( "Equal found: %s\n", yytext); }
"!="					{ printf( "NotEqual found: %s\n", yytext); }
"<"						{ printf( "Lesser found: %s\n", yytext); }
"<="					{ printf( "LesserEqual found: %s\n", yytext); }
">"						{ printf( "Greater found: %s\n", yytext); }
">="					{ printf( "GreaterEqual found: %s\n", yytext); }
"+"						{ printf( "Add found: %s\n", yytext); }
"-"						{ printf( "Sub found: %s\n", yytext); }
"*"						{ printf( "Mul found: %s\n", yytext); }
"/"						{ printf( "Div found: %s\n", yytext); }
"%"						{ printf( "Mod found: %s\n", yytext); }
"!"						{ printf( "Exmark found: %s\n", yytext); }
","						{ printf( "Comma found: %s\n", yytext); }
"#"[^\n]\n				{ printf( "Comment found: %s\n", yytext); }
.						{ printf( "Unknown found: %s\n", yytext); }

%%
     
int main( int argc, char **argv )
    {
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;
     
    yylex();
    }
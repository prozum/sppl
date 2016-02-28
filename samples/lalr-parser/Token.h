//
// Created by Hejsil on 2/26/2016.
//

#ifndef LALR_PARSER_TOKEN_H
#define LALR_PARSER_TOKEN_H

#include <string>
#include <unordered_map>

namespace scanner {
    enum Lexeme {
        ID,
        ARROW,
        PIPE,
        PARSTART,
        PAREND,
        SQSTART,
        SQEND,
        COMMA,
        OR,
        AND,
        ASSIGN,
        EQUAL,
        NOTEQUAL,
        LESSER,
        GREATER,
        LESSEREQUAL,
        GREATEREQUAL,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        EXMARK,
        COLON,
        INTEGERLIT,
        FLOATLIT,
        BOOLEANLIT,
        CHARLIT,
        STRINGLIT,
        INT,
        FLOAT,
        BOOL,
        CHAR,
        DEF,
        STRING,
        IGNORE,
        UNKNOWN
    };

    class Token {
    public:
		Token() { };
		Token(Lexeme l, int lnum) : Token(l, "", lnum) { };
		Token(Lexeme l, std::string s, int lnum) { lex = l; value = s; line_number = lnum; };
		Token(Lexeme l, char c, int lnum) { lex = l; value = c; line_number = lnum; };

        Lexeme lex;
        std::string value;
		int line_number;
    };
}

#endif //LALR_PARSER_TOKEN_H

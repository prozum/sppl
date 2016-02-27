//
// Created by Hejsil on 2/26/2016.
//

#ifndef LALR_PARSER_TOKEN_H
#define LALR_PARSER_TOKEN_H

#include <bits/stringfwd.h>
#include <bits/basic_string.h>

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
        Token();
        Token(Lexeme l, std::string s) { lex = l; value = s; };
        Token(Lexeme l, char c) { lex = l; value = c; };
        Lexeme lex;
        std::string value;
    };
}

#endif //LALR_PARSER_TOKEN_H

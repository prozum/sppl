#pragma once

#include <string>
#include <iostream>
#include <iostream>
#include <memory>
#include <map>

enum TokenType
{
    TYPE,
    IDENTIFIER,

    BOOLEAN,
    INTEGER,
    DECIMAL,
    STRING,
    CHAR,

    PAR_START,
    PAR_END,
    SQR_START,
    SQR_END,

    ASSIGN,

    SEMI,
    COMMA,
    DOT,

    NEWLINE,

    UNKNOWN
};

struct Token
{
    Token() {};
    Token(TokenType type, std::string value)
    {
        Type = type;
        Value = value;
    }

    TokenType Type;
    std::string Value;
};

class Scanner
{
public:
    Scanner(std::istream &istream);
    virtual ~Scanner();

    virtual Token Scan();

    Token CurToken;

private:
    void SkipWhiteSpace();
    std::string NextChar();
    void SetToken(TokenType type, std::string value = "");

    void ParseNumber();
    void ParseString();
    void ParseIdentifier();

    std::istream  &_istream;
    std::string CurChar;
    std::map<std::string,Token> KeywordMap;
};

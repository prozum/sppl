#include "Scanner.h"

Scanner::Scanner(std::istream &istream)
        : _istream(istream)
{
    KeywordMap["Int"] = Token(TokenType::TYPE, "Int");
    KeywordMap["Dec"] = Token(TokenType::TYPE, "Dec");
    KeywordMap["Bool"] = Token(TokenType::TYPE, "Bool");
    KeywordMap["String"] = Token(TokenType::TYPE, "String");
}

Scanner::~Scanner() { }

void Scanner::SkipWhiteSpace()
{
    while (isspace(_istream.peek()))
        _istream.get();
}

std::string Scanner::NextChar()
{
    CurChar = _istream.get();

    while ((_istream.peek() & 0xc0) == 0x80)
        CurChar += _istream.get();

    return CurChar;
}

void Scanner::SetToken(TokenType type, std::string value)
{
    CurToken.Type = type;
    if (!value.empty())
        CurToken.Value = value;
}

Token Scanner::Scan()
{
    NextChar();
    SkipWhiteSpace();

    switch (CurChar[0])
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ParseNumber();
            break;
        case '"':
            ParseString();
            break;
        case '\n':
            SetToken(TokenType::NEWLINE);
            break;
        case '=':
            SetToken(TokenType::ASSIGN);
            break;
        case ';':
            SetToken(TokenType::SEMI);
            break;
        case '.':
            SetToken(TokenType::DOT);
            break;
        case ',':
            SetToken(TokenType::COMMA);
            break;
        case '(':
            SetToken(TokenType::PAR_START);
            break;
        case ')':
            SetToken(TokenType::PAR_END);
            break;
        case '[':
            SetToken(TokenType::SQR_START);
            break;
        case ']':
            SetToken(TokenType::SQR_END);
            break;
        default:
            ParseIdentifier();
    }

    return CurToken;
}

void Scanner::ParseNumber()
{
}

void Scanner::ParseString()
{
    std::string str;

    NextChar();

    while (CurChar[0]!='"')
        str += NextChar();
}

void Scanner::ParseIdentifier()
{
    std::string id;

    while (!isspace(CurChar[0]))
    {
        id += CurChar;
        NextChar();
    }

    if (KeywordMap.count(id))
    {
        CurToken = KeywordMap[id];
        return;
    }

    SetToken(TokenType::IDENTIFIER, id);
}


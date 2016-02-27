//
// Created by Hejsil on 2/26/2016.
//

#include <ctype.h>
#include "Scanner.h"

std::vector<scanner::Token>* scanner::Scanner::scan(std::string str) {
    auto res = new std::vector<Token>();
    Token token;
    eat();

    while (string.length() != 0){
        if (isalpha(current) || current == '_'){
            token = scan_identifier();
        } else if (isdigit(current)){
            token = scan_number();
        } else {
            switch (current){
                case ':':
                    token = eat(COLON, current);
                    break;
                case '-':
                    token = eat(SUB, current);

                    if (current == '>'){
                        token.value += current;
                        token.lex = ARROW;
                        eat();
                    }
                case '|':
                    token = eat(PIPE, current);

                    if (current == '|'){
                        token.value += current;
                        token.lex = OR;
                        eat();
                    }
                    break;
                case '=':
                    token = eat(ASSIGN, current);

                    if (current == '=') {
                        token.value += current;
                        token.lex = EQUAL;
                        eat();
                    }
                    break;
                case '(':
                    token = eat(PARSTART, current);
                    break;
                case ')':
                    token = eat(PAREND, current);
                    break;
                case '[':
                    token = eat(SQSTART, current);
                    break;
                case ']':
                    token = eat(SQEND, current);
                    break;
                case '&':
                    token = eat(UNKNOWN, current);

                    if (current == '&') {
                        token.value += current;
                        token.lex = AND;
                        eat();
                    }
                    break;
                case '!':
                    token = eat(EXMARK, current);

                    if (current == '=') {
                        token.value += current;
                        token.lex = NOTEQUAL;
                        eat();
                    }
                    break;
                case '<':
                    token = eat(LESSER, current);

                    if (current == '=') {
                        token.value += current;
                        token.lex = LESSEREQUAL;
                        eat();
                    }
                    break;
                case '>':
                    token = eat(GREATER, current);

                    if (current == '=') {
                        token.value += current;
                        token.lex = GREATEREQUAL;
                        eat();
                    }
                    break;
                case '+':
                    token = eat(ADD, current);
                    break;
                case '*':
                    token = eat(MUL, current);
                    break;
                case '/':
                    token = eat(DIV, current);
                    break;
                case '%':
                    token = eat(MOD, current);
                    break;
                case ',':
                    token = eat(COMMA, current);
                    break;
                case '"':
                    token = scan_string();
                    break;
                case '\'':
                    token = scan_char();
                    break;
                case '\t':
                case '\n':
                    token = eat(IGNORE, current);
                    break;
                case '#':
                    token = scan_comment();
                    break;
                default:
                    token = eat(UNKNOWN, current);
                    break;
            }
        }

        if (token.lex != IGNORE)
            res->push_back(token);
    }
}

void scanner::Scanner::eat() {
    if (string.length() != 0){
        current = string.front();
        string.erase(0);
    } else {
        current = ' ';
    }
}

Token scanner::Scanner::eat(Lexeme token, char cha) {
    Token res = Token(token, cha);
    eat();

    return res;
}

scanner::Token scanner::Scanner::scan_identifier() {
    Token res(ID, "");

    do {
        res.value += current;
        eat();
    } while (isdigit(current) || isalpha(current) || current == '_');

    return res;
}

scanner::Token scanner::Scanner::scan_string() {
    Token res(STRINGLIT, "");
    char prev;

    eat();
    do {
        prev = current;
        res.value += current;
        eat();
    } while (current != '"' || prev == '\\');
    eat();

    return res;
}

scanner::Token scanner::Scanner::scan_char() {
    Token res(CHARLIT, "");
    char prev;

    eat();
    do {
        prev = current;
        res.value += current;
        eat();
    } while (current != '\'' || prev == '\\');
    eat();

    /* check length here maybe? */

    return res;
}

scanner::Token scanner::Scanner::scan_number() {
    Token res(INTEGERLIT, "");

    do {
        res.value += current;
        eat();

        if (current == '.'){
            if (res.lex == INTEGERLIT)
                res.lex = FLOATLIT;
            else
                res.lex = UNKNOWN;

            res.value += current;
            eat();

            if (!isdigit(current))
                res.lex = UNKNOWN;
        }
    } while (isdigit(current));

    return res;
}

scanner::Token scanner::Scanner::scan_comment() {
    Token res(IGNORE, "");

    do {
        res.value += current;
        eat();
    } while (current != '\n');

    return res;

}

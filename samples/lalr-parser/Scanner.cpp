//
// Created by Hejsil on 2/26/2016.
//
#include <string>
#include <unordered_map>
#include "Scanner.h"

std::vector<scanner::Token>* scanner::Scanner::scan(std::string str) {
    auto res = new std::vector<Token>();
    Token token;

	string = str;
    eat();

    while (current != '\0'){
        if (isalpha(current) || current == '_'){
            token = scan_identifier();
        } else if (isdigit(current)){
            token = scan_number();
        } else {
			switch (current) {
			case '"':
				token = scan_string();
			case '\'':
				token = scan_char();
			case '#':
				token = scan_comment();
			default:
				token = scan_other();
				break;
			}
        }

        if (token.lex != IGNORE)
            res->push_back(token);
    }

	return res;
}

scanner::Token scanner::Scanner::scan_other() {
	Token res;
	std::string combined = "";
	combined += current;
	combined += next;
	auto got_double = double_symbols.find(combined);

	if (got_double != double_symbols.end()) {
		res = Token(got_double->second, combined);
		eat(2);
	} else {
		auto got_single = single_symbols.find(current);

		if (got_single != single_symbols.end()) {
			res = Token(got_single->second, current);
		} else {
			res = Token(UNKNOWN, current);
		}

		eat();
	}

	return res;
}

void scanner::Scanner::eat(int n) {
	int length = string.length();
	
	for (size_t i = 0; i < n; i++) {
		if (length - i != 0) {
			current = string.front();
			string.erase(0, 1);

			if (length - (i + 1) != 0)
				next = string.front();
			else
				next = '\0';
		} else {
			current = '\0';
			break;
		}
	}
}

scanner::Token scanner::Scanner::scan_identifier() {
    Token res(ID);

    do {
        res.value += current;
        eat();
    } while (isdigit(current) || isalpha(current) || current == '_');

	auto got_keyword = keywords.find(res.value);

	if (got_keyword != keywords.end()) {
		res.lex = got_keyword->second;
	}

    return res;
}

scanner::Token scanner::Scanner::scan_string() {
    Token res(STRINGLIT);
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
    Token res(CHARLIT);
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
    Token res(INTEGERLIT);

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
    Token res(IGNORE);

    do {
        res.value += current;
        eat();
    } while (current != '\n');

    return res;

}

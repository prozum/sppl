//
// Created by Hejsil on 2/26/2016.
//

#ifndef LALR_PARSER_SCANNER_H
#define LALR_PARSER_SCANNER_H


#include <bits/stl_bvector.h>
#include <bits/stl_iterator.h>
#include <bits/unordered_map.h>
#include "Token.h"

namespace scanner {
    class Scanner {
    private:
        char current;
        std::string string;
        std::unordered_map<std::string, Lexeme> keywords = {
                {"True", BOOLEANLIT },
                {"False", BOOLEANLIT },
                {"Int", INT },
                {"Float", FLOAT },
                {"Bool", BOOL },
                {"Char", CHAR },
                {"String", STRING },
                {"def", DEF }
        };

        void eat();
        Token eat(Lexeme, char);
        Token scan_identifier();
        Token scan_string();
        Token scan_char();
        Token scan_number();
        Token scan_comment();
    public:
        std::vector<Token>* scan(std::string);
    };
}


#endif //LALR_PARSER_SCANNER_H

//
// Created by Hejsil on 2/26/2016.
//

#ifndef LALR_PARSER_SCANNER_H
#define LALR_PARSER_SCANNER_H

#include <vector>
#include "Token.h"

namespace scanner {
    class Scanner {
    private:
        char current;
		char next;
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

		std::unordered_map<char, Lexeme> single_symbols = {
			{ '-' , SUB },
			{ '|' , PIPE },
			{ '"' , STRING },
			{ '=' , ASSIGN },
			{ '!' , EXMARK },
			{ '<' , LESSER },
			{ '>' , GREATER },
			{ ':' , COLON },
			{ '(' , PARSTART },
			{ ')' , PAREND },
			{ '[' , SQSTART },
			{ ']' , SQEND },
			{ '+' , ADD },
			{ '*' , MUL },
			{ '/' , DIV },
			{ '%' , MOD },
			{ ',' , COMMA },
			{ ' ' , IGNORE },
			{ '\t' , IGNORE },
			{ '\n' , IGNORE }
		};

		std::unordered_map<std::string, Lexeme> double_symbols = {
			{ "->" , ARROW },
			{ "||" , OR },
			{ "==" , EQUAL },
			{ "&&" , AND },
			{ "!=" , NOTEQUAL },
			{ "<=" , LESSEREQUAL },
			{ ">=" , GREATEREQUAL }
		};

        void eat(int n = 1);
        Token scan_identifier();
		Token scan_number();
		Token scan_other();
        Token scan_string();
        Token scan_char();
        Token scan_comment();
    public:
        std::vector<Token>* scan(std::string);
    };
}


#endif //LALR_PARSER_SCANNER_H

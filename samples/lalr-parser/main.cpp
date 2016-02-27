#include <iostream>

#include "Ast.H"
#include "Scanner.h"

using namespace std;
using namespace scanner;

unordered_map<Lexeme, std::string> map = {
        { ID, "id" },
        { ARROW, "arrow" },
        { PIPE, "pipe" },
        { PARSTART, "parstart" },
        { PAREND, "parend" },
        { SQSTART, "sqstart" },
        { SQEND, "sqend" },
        { COMMA, "comma" },
        { OR, "or" },
        { AND, "and" },
        { ASSIGN, "assign" },
        { EQUAL, "equal" },
        { NOTEQUAL, "not equal" },
        { LESSER, "lesser" },
        { GREATER, "greater" },
        { LESSEREQUAL, "lesser or equal" },
        { GREATEREQUAL, "greater or equal" },
        { ADD, "add" },
        { SUB, "sub" },
        { MUL, "mul" },
        { DIV, "div" },
        { MOD, "mod" },
        { EXMARK, "exmark" },
        { COLON, "colon" },
        { INTEGERLIT, "int literal" },
        { FLOATLIT, "float literal" },
        { BOOLEANLIT, "bool litetral" },
        { CHARLIT, "char literal" },
        { STRINGLIT, "string literal" },
        { INT, "int" },
        { FLOAT, "float" },
        { BOOL, "bool" },
        { CHAR, "char" },
        { DEF, "def" },
        { STRING, "string" },
        { IGNORE, "ignore" },
        { UNKNOWN, "unknown" }
};

int main() {
    Scanner scanner;
    auto tokens = scanner.scan("def fac: Int -> Int\n"
                                    "\t| 0 = 1\n"
                                    "\t| n = fac(n-1) * n");

    for (auto token : *tokens)
        cout << map[token.lex] << ": " << token.value << endl;

    return 0;
}
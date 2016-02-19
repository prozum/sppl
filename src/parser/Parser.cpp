#include "Parser.h"

Parser::Parser() { }
Parser::~Parser() { }
void Parser::Parse(Scanner &scanner, NodeBuilder &builder)
{
    auto token = scanner.Scan();
}

#include "Compiler.h"

#include "Scanner.h"
#include "NodeBuilder.h"
#include "Parser.h"
#include "CodeGenerator.h"

void Compiler::compile(std::istream &input,
                       std::ostream &output)
{
    Scanner scanner(input);
    NodeBuilder builder;
    Parser parser;
    CodeGenerator generator(output);

    parser.Parse(scanner, builder);

    auto rootNode = builder.GetRootNode();

    rootNode.Traverse(generator);
}

Compiler::Compiler() {

}

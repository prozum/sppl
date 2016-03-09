#include "LLVMCompiler.h"

void LLVMCompiler::compile(std::istream &input,
                          std::ostream &output)
{
    Scanner scanner(input);
    NodeBuilder builder;
    Parser parser;
    CCodeGenerator generator(output);

    parser.Parse(scanner, builder);

    auto rootNode = builder.GetRootNode();

    rootNode.Traverse(generator);
}

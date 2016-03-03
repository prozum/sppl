#include "NodeBuilder.h"



ast::Node* NodeBuilder::GetRootNode()
{
    return new ast::Program();
}

ast::Node* NodeBuilder::NewVariable(std::string variableName) const
{
    return new ast::Id();
}

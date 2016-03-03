#pragma once
#include <string>
#include "Node.h"


class NodeBuilder {

public:
    virtual ast::Node* NewVariable(std::string var) const;

    virtual ast::Node* GetRootNode();
};

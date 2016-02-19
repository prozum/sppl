#pragma once
#include <string>
#include "Node.h"

class NodeBuilder {

public:
    virtual Node NewVariable(std::string variableName) const;

    virtual Node GetRootNode();
};

#pragma once
#include "Scanner.h"
#include "NodeBuilder.h"

class Parser
{
public:
    Parser();
    virtual ~Parser();

    virtual void Parse(Scanner&, NodeBuilder&);
};

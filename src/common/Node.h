#pragma once

#include "Visitor.h"

class Node
{
public:
    Node();
    ~Node();

    virtual void Traverse(Visitor&);
};

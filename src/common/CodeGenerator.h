#pragma once

#include <ostream>
#include "Visitor.h"

class CodeGenerator: public Visitor
{
public:
    CodeGenerator(std::ostream&);

private:
    std::ostream &_ostream;
};

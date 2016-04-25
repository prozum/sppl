#pragma once

#include "CodeGenerator.h"
#include "Node.h"

#include <string>

using namespace std;
using namespace common;
using namespace parser;

namespace codegen {
class Printer : public CodeGenerator {
  public:
    Printer(Driver &Drv);

    virtual void visit(Program &Node);
};
}

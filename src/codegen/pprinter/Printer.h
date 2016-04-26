#pragma once

#include "CodeGenerator.h"
#include "Node.h"

#include <string>

namespace codegen {
class Printer : public parser::CodeGenerator {
  public:
    Printer(parser::Driver &Drv);

    virtual void visit(common::Program &Node);
};
}

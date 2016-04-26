#pragma once

#include "Node.h"
#include "Visitor.h"

#include <ostream>
#include <string>

namespace optimizer {

class GeneralOptimizer : public common::Visitor {
  public:
    void visit(common::Program &Node);

  private:
    void visit(common::Function &Node);
};
}

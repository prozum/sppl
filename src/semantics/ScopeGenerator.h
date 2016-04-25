#pragma once

#include "Node.h"
#include "Printer.h"
#include "Visitor.h"
#include <stack>

namespace semantics {

class ScopeGenerator : public common::Visitor {
  public:
    ScopeGenerator(common::Scope *Scp);

    void visit(common::Program &Node);

  private:
    void visit(common::Function &Node);
    void visit(common::AlgebraicDT &Node);
    void visit(common::Product &Node);

    bool isGeneric(common::Function &);
    bool isGeneric(common::AlgebraicDT &);
    bool isGeneric(common::Type &);

    common::Scope *CurScope;
    common::AlgebraicDT *CurADT;
};
}
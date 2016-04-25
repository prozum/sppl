#pragma once

#include <stack>
#include "Node.h"
#include "Visitor.h"
#include "Printer.h"

using namespace common;
using namespace codegen;

namespace semantics {

    class ScopeGenerator : public Visitor {
    public:
        ScopeGenerator(Scope* Scp);

        void visit(Program &Node);

    private:
        void visit(Function &Node);
        void visit(AlgebraicDT &Node);
        void visit(Product &Node);

        bool isGeneric(Function&);
        bool isGeneric(AlgebraicDT&);
        bool isGeneric(Type&);

        Scope *CurScope;
        AlgebraicDT *CurADT;
    };
}
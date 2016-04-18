#pragma once

#include "Visitor.h"
#include "Node.h"

#include <ostream>
#include <string>

using namespace common;

namespace optimizer {

    class GeneralOptimizer : public Visitor {
    public:
        void visit(Program &Node);
        void visit(Function &Node);
        void visit(Case &Node);
        void visit(Or &Node);
        void visit(And &Node);
        void visit(Equal &Node);
        void visit(NotEqual &Node);
        void visit(Lesser &Node);
        void visit(Greater &Node);
        void visit(LesserEq &Node);
        void visit(GreaterEq &Node);
        void visit(Add &Node);
        void visit(Sub &Node);
        void visit(Mul &Node);
        void visit(Div &Node);
        void visit(Mod &Node);
        void visit(ListAdd &Node);
        void visit(Par &Node);
        void visit(Not &Node);
        void visit(Int &Node);
        void visit(Float &Node);
        void visit(Bool &Node);
        void visit(Char &Node);
        void visit(String &Node);
        void visit(ListPattern &Node);
        void visit(TuplePattern &Node);
        void visit(ListSplit &Node);
        void visit(List &Node);
        void visit(Tuple &Node);
        void visit(Id &Node);
        void visit(Call &Node);
        void visit(Type &Node);

    private:
    };
}

#pragma once

#include <stack>
#include "Node.h"
#include "Visitor.h"

using namespace common;

namespace semantics {

    class PatternChecker : public Visitor {
    public:
        PatternChecker();

        void visit(Program &node);

        void visit(Function &node);

        void visit(Case &node);

    private:
        Function *cur_func;
    };
}

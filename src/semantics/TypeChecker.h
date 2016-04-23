#pragma once

#include "Visitor.h"

#include <stack>
#include <vector>
#include <Node.h>

using namespace common;
using namespace std;

namespace semantics {
    class TypeChecker : public Visitor {
    public:
        bool IgnoreGenerics = false;

        TypeChecker();

        virtual void visit(Program &Node);
        virtual void visit(Function &Node);
        virtual void visit(Case &Node);
        virtual void visit(Or &Node);
        virtual void visit(And &Node);
        virtual void visit(Equal &Node);
        virtual void visit(NotEqual &Node);
        virtual void visit(Lesser &Node);
        virtual void visit(Greater &Node);
        virtual void visit(LesserEq &Node);
        virtual void visit(GreaterEq &Node);
        virtual void visit(Add &Node);
        virtual void visit(Sub &Node);
        virtual void visit(Mul &Node);
        virtual void visit(Div &Node);
        virtual void visit(Mod &Node);
        virtual void visit(ListAdd &Node);
        virtual void visit(ParExpr &Node);
        virtual void visit(Not &Node);
        virtual void visit(IntPattern &Node);
        virtual void visit(FloatPattern &Node);
        virtual void visit(CharPattern &Node);
        virtual void visit(BoolPattern &Node);
        virtual void visit(ListPattern &Node);
        virtual void visit(TuplePattern &Node);
        virtual void visit(ListSplit &Node);
        virtual void visit(ListExpr &Node);
        virtual void visit(TupleExpr &Node);
        virtual void visit(CharExpr &Node);
        virtual void visit(FloatExpr &Node);
        virtual void visit(IntExpr &Node);
        virtual void visit(IdPattern &Node);
        virtual void visit(IdExpr &Node);
        virtual void visit(CallExpr &Node);

    private:
        Function *CurFunc;
    };
}

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
        void visit(IntPattern &Node);
        void visit(FloatPattern &Node);
        void visit(CharPattern &Node);
        void visit(ParExpr &Node);
        void visit(Not &Node);
        void visit(ListPattern &Node);
        void visit(TuplePattern &Node);
        void visit(ListSplit &Node);
        void visit(ListExpr &Node);
        void visit(TupleExpr &Node);
        void visit(IdPattern &Node);
        void visit(CallExpr &Node);
        void visit(Type &Node);
        void visit(IdExpr &Node);
        void visit(IntExpr &Node);
        void visit(FloatExpr &Node);
        void visit(CharExpr &Node);

    private:
        Scope *CurScope;
        Function *CurFunc;
    };
}
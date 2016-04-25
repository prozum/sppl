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
        void visit(Case &Node);
        void visit(LambdaArg &Node);
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
        void visit(ProducerConsumer &Node);
        void visit(Concat &Node);
        void visit(To &Node);
        void visit(ParExpr &Node);
        void visit(Not &Node);
        void visit(Negative &Node);
        void visit(LambdaFunction &Node);
        void visit(IdPattern &Node);
        void visit(IntPattern &Node);
        void visit(FloatPattern &Node);
        void visit(CharPattern &Node);
        void visit(BoolPattern &Node);
        void visit(StringPattern &Node);
        void visit(ListPattern &Node);
        void visit(TuplePattern &Node);
        void visit(ListSplit &Node);
        void visit(WildPattern &Node);
        void visit(AlgebraicPattern &Node);
        void visit(ParPattern &Node);
        void visit(IdExpr &Node);
        void visit(IntExpr &Node);
        void visit(FloatExpr &Node);
        void visit(CharExpr &Node);
        void visit(BoolExpr &Node);
        void visit(ListExpr &Node);
        void visit(TupleExpr &Node);
        void visit(CallExpr &Node);
        void visit(StringExpr &Node);
        void visit(AlgebraicExpr &Node);

        Scope *CurScope;
        Function *CurFunc;
    };
}
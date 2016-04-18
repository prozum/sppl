#include "ParallelOptimizer.h"

#include <iostream>
#include <string>

using namespace std;
using namespace common;

namespace optimizer {

    void ParallelOptimizer::visit(Program &Node)
    {
        for (auto &Func : Node.Decls){
            Func->accept(*this);
        }
    }

    void ParallelOptimizer::visit(Function &Node)
    {
        for (auto &Case : Node.Cases) {
            Case->accept(*this);
        }

    }

    void ParallelOptimizer::visit(Case &Node)
    {
        for (auto &Pattern : Node.Patterns) {
            Pattern->accept(*this);
        }
    }

    void ParallelOptimizer::visit(Or &Node)
    {

    }

    void ParallelOptimizer::visit(And &Node)
    {

    }

    void ParallelOptimizer::visit(Equal &Node)
    {

    }

    void ParallelOptimizer::visit(NotEqual &Node)
    {

    }

    void ParallelOptimizer::visit(Lesser &Node)
    {

    }

    void ParallelOptimizer::visit(LesserEq &Node)
    {

    }

    void ParallelOptimizer::visit(Greater &Node)
    {

    }

    void ParallelOptimizer::visit(GreaterEq &Node)
    {

    }

    void ParallelOptimizer::visit(Add &Node)
    {

    }

    void ParallelOptimizer::visit(Sub &Node)
    {

    }

    void ParallelOptimizer::visit(Mul &Node)
    {

    }

    void ParallelOptimizer::visit(Div &Node)
    {

    }

    void ParallelOptimizer::visit(Mod &Node)
    {

    }

    void ParallelOptimizer::visit(ListAdd &Node)
    {

    }

    void ParallelOptimizer::visit(Par &Node)
    {

    }

    void ParallelOptimizer::visit(Not &Node)
    {

    }

    void ParallelOptimizer::visit(ListPattern &Node)
    {

    }

    void ParallelOptimizer::visit(TuplePattern &Node)
    {

    }

    void ParallelOptimizer::visit(ListSplit &Node)
    {

    }

    void ParallelOptimizer::visit(IntPattern &Node)
    {
    }

    void ParallelOptimizer::visit(FloatPattern &Node)
    {
    }

    void ParallelOptimizer::visit(Bool &Node)
    {
    }

    void ParallelOptimizer::visit(CharPattern &Node)
    {
    }

    void ParallelOptimizer::visit(String &Node)
    {
    }

    void ParallelOptimizer::visit(ListExpression &Node)
    {

    }

    void ParallelOptimizer::visit(IdPattern &Node)
    {
    }

    void ParallelOptimizer::visit(Call &Node)
    {

    }

    void ParallelOptimizer::visit(Type &Node)
    {
    }

    void ParallelOptimizer::visit(TupleExpression &Node) {

    }
}

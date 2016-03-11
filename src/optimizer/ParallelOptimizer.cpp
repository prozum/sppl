#include "ParallelOptimizer.h"

#include <iostream>
#include <string>

using namespace std;
using namespace common;

namespace optimizer {

    void ParallelOptimizer::visit(Program &node)
    {
        for (auto f : node.funcs){
            f->accept(*this);
        }
    }

    void ParallelOptimizer::visit(Function &node)
    {
        for (auto c : node.cases) {
            c->accept(*this);
        }

    }

    void ParallelOptimizer::visit(Case &node)
    {
        for (auto p : node.patterns) {
            p->accept(*this);
        }
    }

    void ParallelOptimizer::visit(Or &node)
    {

    }

    void ParallelOptimizer::visit(And &node)
    {

    }

    void ParallelOptimizer::visit(Equal &node)
    {

    }

    void ParallelOptimizer::visit(NotEqual &node)
    {

    }

    void ParallelOptimizer::visit(Lesser &node)
    {

    }

    void ParallelOptimizer::visit(LesserEq &node)
    {

    }

    void ParallelOptimizer::visit(Greater &node)
    {

    }

    void ParallelOptimizer::visit(GreaterEq &node)
    {

    }

    void ParallelOptimizer::visit(Add &node)
    {

    }

    void ParallelOptimizer::visit(Sub &node)
    {

    }

    void ParallelOptimizer::visit(Mul &node)
    {

    }

    void ParallelOptimizer::visit(Div &node)
    {

    }

    void ParallelOptimizer::visit(Mod &node)
    {

    }

    void ParallelOptimizer::visit(ListAdd &node)
    {

    }

    void ParallelOptimizer::visit(Par &node)
    {

    }

    void ParallelOptimizer::visit(Not &node)
    {

    }

    void ParallelOptimizer::visit(ListPattern &node)
    {

    }

    void ParallelOptimizer::visit(TuplePattern &node)
    {

    }

    void ParallelOptimizer::visit(ListSplit &node)
    {

    }

    void ParallelOptimizer::visit(Int &node)
    {
    }

    void ParallelOptimizer::visit(Float &node)
    {
    }

    void ParallelOptimizer::visit(Bool &node)
    {
    }

    void ParallelOptimizer::visit(Char &node)
    {
    }

    void ParallelOptimizer::visit(String &node)
    {
    }

    void ParallelOptimizer::visit(List &node)
    {

    }

    void ParallelOptimizer::visit(Id &node)
    {
    }

    void ParallelOptimizer::visit(Call &node)
    {

    }

    void ParallelOptimizer::visit(Type &node)
    {
    }

    void ParallelOptimizer::visit(Tuple &node) {

    }
}

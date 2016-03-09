#include "ParallelOptimizer.h"

#include <iostream>
#include <string>

using namespace std;

namespace optimizer {

    void Optimizer::visit(Program *node)
    {
        for (auto f : node->funcs){
            f->accept(this);
        }
    }

    void Optimizer::visit(Function *node)
    {
        for (auto c : node->cases) {
            c->accept(this);
        }
    }

    void Optimizer::visit(Case *node)
    {
        for (auto p : node->patterns) {
            p->accept(this);
        }
    }

    void Optimizer::visit(Or *node)
    {

    }

    void Optimizer::visit(And *node)
    {

    }

    void Optimizer::visit(Equal *node)
    {

    }

    void Optimizer::visit(NotEqual *node)
    {

    }

    void Optimizer::visit(Lesser *node)
    {

    }

    void Optimizer::visit(LesserEq *node)
    {

    }

    void Optimizer::visit(Greater *node)
    {

    }

    void Optimizer::visit(GreaterEq *node)
    {

    }

    void Optimizer::visit(Add *node)
    {

    }

    void Optimizer::visit(Sub *node)
    {

    }

    void Optimizer::visit(Mul *node)
    {

    }

    void Optimizer::visit(Div *node)
    {

    }

    void Optimizer::visit(Mod *node)
    {

    }

    void Optimizer::visit(ListAdd *node)
    {

    }

    void Optimizer::visit(Par *node)
    {

    }

    void Optimizer::visit(Not *node)
    {

    }

    void Optimizer::visit(ListPattern *node)
    {

    }

    void Optimizer::visit(TuplePattern *node)
    {

    }

    void Optimizer::visit(ListSplit *node)
    {

    }

    void Optimizer::visit(Int *node)
    {
    }

    void Optimizer::visit(Float *node)
    {
    }

    void Optimizer::visit(Bool *node)
    {
    }

    void Optimizer::visit(Char *node)
    {
    }

    void Optimizer::visit(String *node)
    {
    }

    void Optimizer::visit(List *node)
    {

    }

    void Optimizer::visit(Id *node)
    {
    }

    void Optimizer::visit(Call *node)
    {

    }

    void Optimizer::visit(Type *node)
    {
    }
}

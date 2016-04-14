#include "GeneralOptimizer.h"

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;
using namespace common;

namespace optimizer {

    void GeneralOptimizer::visit(Program &node)
    {
        for (auto f : node.funcs){
            f->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Function &node)
    {
        for (auto c : node.cases) {
            auto expr = c->expr.get();

            if (typeid(*expr) == typeid(Call) &&
                typeid(*((Call*)expr)->callee) == typeid(Id) &&
                ((Id*)((Call*)expr)->callee.get())->id == node.id) {
                c->tail_rec = true;
            }

            c->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Case &node)
    {
        for (auto p : node.patterns) {
            p->accept(*this);
        }

        node.expr->accept(*this);
    }

    void GeneralOptimizer::visit(Or &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(And &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Equal &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(NotEqual &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Lesser &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(LesserEq &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Greater &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(GreaterEq &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Add &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Sub &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Mul &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Div &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Mod &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(ListAdd &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Par &node)
    {
        node.child->accept(*this);
    }

    void GeneralOptimizer::visit(Not &node)
    {
        node.child->accept(*this);
    }

    void GeneralOptimizer::visit(ListPattern &node)
    {
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
    }

    void GeneralOptimizer::visit(TuplePattern &node)
    {
        for (auto pattern : node.patterns) {
            pattern->accept(*this);
        }
    }

    void GeneralOptimizer::visit(ListSplit &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void GeneralOptimizer::visit(Int &node)
    {
    }

    void GeneralOptimizer::visit(Float &node)
    {
    }

    void GeneralOptimizer::visit(Bool &node)
    {
    }

    void GeneralOptimizer::visit(Char &node)
    {
    }

    void GeneralOptimizer::visit(String &node)
    {
    }

    void GeneralOptimizer::visit(List &node)
    {
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Id &node)
    {
    }

    void GeneralOptimizer::visit(Call &node)
    {
        node.callee->accept(*this);

        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Type &node)
    {
    }

    void GeneralOptimizer::visit(Tuple &node)
    {
        for (auto expr : node.exprs) {
            expr->accept(*this);
        }
    }
}

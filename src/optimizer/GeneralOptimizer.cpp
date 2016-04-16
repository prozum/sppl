#include "GeneralOptimizer.h"

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;
using namespace common;

namespace optimizer {

    void GeneralOptimizer::visit(Program &node)
    {
        for (auto &f: node.Funcs){
            f->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Function &node)
    {
        for (auto &c: node.Cases) {
            auto expr = c->Expr.get();

            if (typeid(*expr) == typeid(Call) &&
                typeid(*((Call*)expr)->Callee) == typeid(Id) &&
                ((Id*)((Call*)expr)->Callee.get())->Val == node.Id) {
                c->TailRec = true;
            }

            c->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Case &node)
    {
        for (auto &p: node.Patterns) {
            p->accept(*this);
        }

        node.Expr->accept(*this);
    }

    void GeneralOptimizer::visit(Or &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(And &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Equal &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(NotEqual &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Lesser &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(LesserEq &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Greater &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(GreaterEq &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Add &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Sub &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Mul &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Div &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Mod &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(ListAdd &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
    }

    void GeneralOptimizer::visit(Par &node)
    {
        node.Child->accept(*this);
    }

    void GeneralOptimizer::visit(Not &node)
    {
        node.Child->accept(*this);
    }

    void GeneralOptimizer::visit(ListPattern &node)
    {
        for (auto &pattern: node.Patterns) {
            pattern->accept(*this);
        }
    }

    void GeneralOptimizer::visit(TuplePattern &node)
    {
        for (auto &pattern: node.Patterns) {
            pattern->accept(*this);
        }
    }

    void GeneralOptimizer::visit(ListSplit &node)
    {
        node.Left->accept(*this);
        node.Right->accept(*this);
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
        for (auto &expr: node.Elements) {
            expr->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Id &node)
    {
    }

    void GeneralOptimizer::visit(Call &node)
    {
        node.Callee->accept(*this);

        for (auto &expr: node.Args) {
            expr->accept(*this);
        }
    }

    void GeneralOptimizer::visit(Type &node)
    {
    }

    void GeneralOptimizer::visit(Tuple &node)
    {
        for (auto &expr: node.Elements) {
            expr->accept(*this);
        }
    }
}

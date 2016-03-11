#include "GeneralOptimizer.h"

#include <iostream>
#include <string>

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

            // first checks if the case's expression is only a function call
            // secondly checks if the callee of the function call is an Id
            // lastly check is the function call's id is matching the case's parent function
            if (typeid(c->expr) == typeid(Call)
                && typeid(((Call)c->expr).callee) == typeid(Id)
                && (((Id)((Call)c->expr).callee)).id == node.id) {
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
    }

    void GeneralOptimizer::visit(Or &node)
    {

    }

    void GeneralOptimizer::visit(And &node)
    {

    }

    void GeneralOptimizer::visit(Equal &node)
    {

    }

    void GeneralOptimizer::visit(NotEqual &node)
    {

    }

    void GeneralOptimizer::visit(Lesser &node)
    {

    }

    void GeneralOptimizer::visit(LesserEq &node)
    {

    }

    void GeneralOptimizer::visit(Greater &node)
    {

    }

    void GeneralOptimizer::visit(GreaterEq &node)
    {

    }

    void GeneralOptimizer::visit(Add &node)
    {

    }

    void GeneralOptimizer::visit(Sub &node)
    {

    }

    void GeneralOptimizer::visit(Mul &node)
    {

    }

    void GeneralOptimizer::visit(Div &node)
    {

    }

    void GeneralOptimizer::visit(Mod &node)
    {

    }

    void GeneralOptimizer::visit(ListAdd &node)
    {

    }

    void GeneralOptimizer::visit(Par &node)
    {

    }

    void GeneralOptimizer::visit(Not &node)
    {

    }

    void GeneralOptimizer::visit(ListPattern &node)
    {

    }

    void GeneralOptimizer::visit(TuplePattern &node)
    {

    }

    void GeneralOptimizer::visit(ListSplit &node)
    {

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

    }

    void GeneralOptimizer::visit(Id &node)
    {
    }

    void GeneralOptimizer::visit(Call &node)
    {

    }

    void GeneralOptimizer::visit(Type &node)
    {
    }

    void GeneralOptimizer::visit(Tuple &node) {

    }
}

#include "Profiler.h"

#include <random>

using namespace interpreter;
using namespace common;
using namespace std;


namespace profiler {

    Profiler::Profiler(Program &node) {
        this->node = node;
    }

    void Profiler::instruction_time_profile(int runs)
    {
        mode = INSTRUCTION_TIME;

        node->accept(this);
    }

    void Profiler::user_time_profile(int runs)
    {
        mode = USER_TIME;

        node->accept(this);
    }

    void Profiler::visit(Program *node)
    {
        for (auto f : node->funcs) {
            f->accept(this);
        }
    }

    void Profiler::visit(Function *node)
    {
        for (auto c : node->cases) {
            c->accept(this);
        }
    }

    void Profiler::visit(Case *node)
    {
        if (mode == INSTRUCTION_TIME) {

        } else if (mode == USER_TIME) {
            // tbd
            return;
        }
    }

    void Profiler::visit(Or *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value || ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(And *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value && ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(Equal *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value == ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(NotEqual *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value != ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(Lesser *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value < ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(LesserEq *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value <= ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(Greater *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value > ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(GreaterEq *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value >= ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(Add *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value >= ((Bool*)node->left->val)->value);
    }

    void Profiler::visit(Sub *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        if (node->left->node_type->type == Int) {
            node->val = new Int(((Int*)node->left->val)->value - ((Int*)node->left->val)->value);
        } else {
            node->val = new Float(((Float*)node->left->val)->value - ((Float*)node->left->val)->value);
        }
    }

    void Profiler::visit(Mul *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        if (node->left->node_type->type == Int) {
            node->val = new Int(((Int*)node->left->val)->value * ((Int*)node->left->val)->value);
        } else {
            node->val = new Float(((Float*)node->left->val)->value * ((Float*)node->left->val)->value);
        }
    }

    void Profiler::visit(Div *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        if (node->left->node_type->type == Int) {
            node->val = new Int(((Int*)node->left->val)->value / ((Int*)node->left->val)->value);
        } else {
            node->val = new Float(((Float*)node->left->val)->value / ((Float*)node->left->val)->value);
        }
    }

    void Profiler::visit(Mod *node)
    {
        if (node->left->node_type->type == Int) {
            node->val = new Int(((Int*)node->left->val)->value % ((Int*)node->left->val)->value);
        } else {
            node->val = new Float(((Float*)node->left->val)->value % ((Float*)node->left->val)->value);
        }
    }

    void Profiler::visit(ListAdd *node)
    {

    }

    void Profiler::visit(Par *node)
    {
        node->child->accept(this);
        node->val = node->child;
    }

    void Profiler::visit(Not *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(!((Bool*)node->child->val)->value);
    }

    void Profiler::visit(ListPattern *node)
    {

    }

    void Profiler::visit(TuplePattern *node)
    {

    }

    void Profiler::visit(ListSplit *node)
    {

    }

    void Profiler::visit(Int *node)
    {
        node->val = node;
    }

    void Profiler::visit(Float *node)
    {
        node->val = node;
    }

    void Profiler::visit(Bool *node)
    {
        node->val = node;
    }

    void Profiler::visit(Char *node)
    {
        node->val = node;
    }

    void Profiler::visit(String *node)
    {
        node->val = node;
    }

    void Profiler::visit(List *node)
    {
        node->val = node;
    }

    void Profiler::visit(Id *node)
    {
        node->val = node;
    }

    void Profiler::visit(Call *node)
    {
        node->val = node;
    }

    void Profiler::visit(Type *node)
    {
        node->val = node;
    }
}

#include "Profiler.h"

#include <random>

using namespace interpreter;
using namespace common;
using namespace std;

namespace profiler {

    Profiler::Profiler(Program &node) {
        this->node = &node;
    }

    void Profiler::instruction_time_profile(int runs)
    {
        mode = INSTRUCTION_TIME;

        node->accept(*this);
    }

    void Profiler::user_time_profile(int runs)
    {
        mode = USER_TIME;

        node->accept(*this);
    }

    void Profiler::visit(Program &node)
    {
        for (auto f : node.funcs) {
            f->accept(*this);
        }
    }

    void Profiler::visit(Function &node)
    {
        for (auto c : node.cases) {
            c->accept(*this);
        }
    }

    void Profiler::visit(Case &node)
    {
        if (mode == INSTRUCTION_TIME) {

        } else if (mode == USER_TIME) {
            // tbd
            return;
        }
    }

    void Profiler::visit(Or &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool&)node.left->val).value || ((Bool&)node.left->val).value);
    }

    void Profiler::visit(And &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value && ((Bool &)node.left->val).value);
    }

    void Profiler::visit(Equal &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value == ((Bool &)node.left->val).value);
    }

    void Profiler::visit(NotEqual &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value != ((Bool &)node.left->val).value);
    }

    void Profiler::visit(Lesser &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value < ((Bool &)node.left->val).value);
    }

    void Profiler::visit(LesserEq &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value <= ((Bool &)node.left->val).value);
    }

    void Profiler::visit(Greater &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value > ((Bool &)node.left->val).value);
    }

    void Profiler::visit(GreaterEq &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value >= ((Bool &)node.left->val).value);
    }

    void Profiler::visit(Add &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool &)node.left->val).value >= ((Bool &)node.left->val).value);
    }

    void Profiler::visit(Sub &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        if (node.left->node_type->type == Types::INT) {
            node.val = new Int(((Int &)node.left->val).value - ((Int &)node.left->val).value);
        } else {
            node.val = new Float(((Float &)node.left->val).value - ((Float &)node.left->val).value);
        }
    }

    void Profiler::visit(Mul &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        if (node.left->node_type->type == Types::INT) {
            node.val = new Int(((Int &)node.left->val).value * ((Int &)node.left->val).value);
        } else {
            node.val = new Float(((Float &)node.left->val).value * ((Float &)node.left->val).value);
        }
    }

    void Profiler::visit(Div &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        if (node.left->node_type->type == Types::INT) {
            node.val = new Int(((Int &)node.left->val).value / ((Int &)node.left->val).value);
        } else {
            node.val = new Float(((Float &)node.left->val).value / ((Float &)node.left->val).value);
        }
    }

    void Profiler::visit(Mod &node)
    {
        if (node.left->node_type->type == Types::INT) {
            node.val = new Int(((Int &)node.left->val).value % ((Int &)node.left->val).value);
        } else {
            //node.val = new Float(((Float &)node.left->val).value % ((Float &)node.left->val).value);
        }
    }

    void Profiler::visit(ListAdd &node)
    {

    }

    void Profiler::visit(Par &node)
    {
        node.child->accept(*this);
        node.val = node.child;
    }

    void Profiler::visit(Not &node)
    {
        node.child->accept(*this);

        node.val = new Bool(!((Bool &)node.child->val).value);
    }

    void Profiler::visit(ListPattern &node)
    {

    }

    void Profiler::visit(TuplePattern &node)
    {

    }

    void Profiler::visit(ListSplit &node)
    {

    }

    void Profiler::visit(Int &node)
    {
        node.val = &node;
    }

    void Profiler::visit(Float &node)
    {
        node.val = &node;
    }

    void Profiler::visit(Bool &node)
    {
        node.val = &node;
    }

    void Profiler::visit(Char &node)
    {
        node.val = &node;
    }

    void Profiler::visit(String &node)
    {
        node.val = &node;
    }

    void Profiler::visit(List &node)
    {
        node.val = &node;
    }

    void Profiler::visit(Id &node)
    {
        node.val = &node;
    }

    void Profiler::visit(Call &node)
    {
        node.val = &node;
    }

    void Profiler::visit(Type &node)
    {
        //node.val = &node;
    }

    Pattern &Profiler::generate_literal(Types t)
    {
        switch (t) {
            case INT:
                return Int(rand()) &i;
            case FLOAT:
                return Float(stod(to_string(rand()) + "." + to_string(rand()))) &f;
            case CHAR:
                return Char((char)rand()) &c;
            default:
                return Pattern() &e;
        }
    }

    Pattern &Profiler::generate_list(Type &t, int len){
        auto lp = ListPattern();

        switch (t.types[0]->node_type->type) {
            case INT:
                for (int i = 0; i < len; ++i) {
                    lp.patterns.push_back(&generate_literal(t.node_type->type));
                }
                return lp;
            case FLOAT:
                for (int i = 0; i < len; ++i) {
                    lp.patterns.push_back(&generate_literal(t.node_type->type));
                }
                return lp;
            case CHAR:
                for (int i = 0; i < len; ++i) {
                    lp.patterns.push_back(&generate_literal(t.node_type->type));
                }
                return lp;
            case LIST:
                for (int i = 0; i < len; ++i) {
                    lp.patterns.push_back(&generate_list(*t.types[0]->types[0], len));
                }
                return lp;
            case TUPLE:
                for (int i = 0; i < len; ++i) {
                    lp.patterns.push_back(&generate_tuple(t.types[0]->types));
                }
                return lp;
            default:
                return Pattern() &e;
        }
    }

    Pattern &Profiler::generate_tuple(vector<Type *> ts){
        auto tp = TuplePattern();

        for (int i = 0; i < ts.size(); ++i) {
            switch (ts[i]->node_type->type) {
                case INT:
                    tp.patterns.push_back(&generate_literal(ts[i]->node_type->type));
                    break;
                case FLOAT:
                    tp.patterns.push_back(&generate_literal(ts[i]->node_type->type));
                    break;
                case CHAR:
                    tp.patterns.push_back(&generate_literal(ts[i]->node_type->type));
                    break;
                case LIST:
                    tp.patterns.push_back(&generate_list(*ts[i]->types[0], 10));
                    break;
            }
        }

        return tp;
    }
}

#include "Profiler.h"

using namespace common;
using namespace std;
using namespace interpreter;

namespace profiler {

    Profiler::Profiler(Program &node) {
        this->node = node;
    }

    string Profiler::interpret(string func_id = "main") {
        result = "";

        this->func_id = func_id;
        node->accept(this);

        for (auto f : node->funcs) {
            if (f->id == id) {
                this->set_result(f->val);
            }
        }

        return result;
    }

    void Profiler::set_result(Expr *exp) {
        switch (exp->node_type->type) {
        case INT:
            result += to_string(((Int*)exp)->value);
            break;
        case FLOAT:
            result += to_string(((Float*)exp)->value);
            break;
        case BOOL:
            result += to_string(((Bool*)exp)->value);
            break;
        case CHAR:
            result += to_string(((Char*)exp)->value);
            break;
        case STRING:
            result += to_string(((String*)exp)->value);
            break;
        case LIST:
            result = "[";
            for (int i = 0; i < ((String*)exp)->value.size()- 1; ++i) {
                result += to_string(((String*)exp)->value[i]);
                result += ",";
            }
            node.types.back()->accept(*this);
            result = "]";
        case LIST:
            result = "[";
            for (int i = 0; i < ((List*)exp)->value.size()- 1; ++i) {
                result += to_string(((List*)exp)->value[i]);
                result += ",";
            }
            node.types.back()->accept(*this);
            result = "]";
            break;
        case TUPLE:
            result = "(";
            for (int i = 0; i < ((Tuple*)exp)->value.size()- 1; ++i) {
                result += to_string(((Tuple*)exp)->value[i]);
                result += ",";
            }
            node.types.back()->accept(*this);
            result = ")";
            break;
        case SIGNATURE:
            result += ((Id*)expr)->id;
        default:
            result = "Error";
        }
    }

    void Profiler::visit(Program *node)
    {
        for (auto f : node->funcs) {
            funcs.insert(f->id, f);
        }

        for (auto f : node->funcs) {
            if (f->id == id) {
                f->accept(this);
            }
        }
    }

    void Profiler::visit(Function *node)
    {
        for (auto c : node->cases) {
            if (c) {

            }
        }
    }

    void Profiler::visit(Case *node)
    {
        /*
        for (auto e : node->expr) {
            e->accept(this);
        }
        */
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

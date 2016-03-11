#include "Interpreter.h"

using namespace common;
using namespace std;

namespace interpreter {

    Interpreter::Interpreter(Program &node) {
        this->node = &node;
    }

    string Interpreter::interpret(string func_id = "main") {
        result = "";

        this->func_id = func_id;
        node->accept(*this);

        for (auto f : node->funcs) {
            /* Not working
            if (f->id == id) {
                this->set_result(f->val);
            }
            */
        }

        return result;
    }

    void Interpreter::set_result(Expr &exp) {
        switch (exp.node_type->type) {
        case INT:
            result += to_string(((Int &)exp).value);
            break;
        case FLOAT:
            result += to_string(((Float &)exp).value);
            break;
        case BOOL:
            result += to_string(((Bool &)exp).value);
            break;
        case CHAR:
            result += to_string(((Char &)exp).value);
            break;
        case STRING:
            result += ((String &)exp).value;
            break;
        case LIST:
            result = "[";
            for (int i = 0; i < ((List &)exp).exprs.size()- 1; ++i) {
                set_result(*((List &)exp).exprs[i]);
                result += ",";
            }
            set_result(*((List &)exp).exprs.back());
            result = "]";
            break;
        case TUPLE:
            result = "(";
            for (int i = 0; i < ((Tuple &)exp).exprs.size()- 1; ++i) {
                set_result(*((Tuple &)exp).exprs[i]);
                result += ",";
            }
            set_result(*((Tuple &)exp).exprs.back());
            result = ")";
            break;
        case SIGNATURE:
            result += ((Id *)exp.val)->id;
        default:
            result = "Error";
        }
    }

    void Interpreter::visit(Program &node)
    {
        for (auto f : node.funcs) {
            funcs.insert({ f->id, f });
        }

        for (auto f : node.funcs) {
            if (/*f->id == func_id compare string */ true) {
                f->accept(*this);
            }
        }
    }

    void Interpreter::visit(Function &node)
    {
        for (auto c : node.cases) {
            if (c) {

            }
        }
    }

    void Interpreter::visit(Case &node)
    {
        
    }

    void Interpreter::visit(Or &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool *)node.left->val)->value || ((Bool *)node.left->val)->value);
    }

    void Interpreter::visit(And &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value && ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(Equal &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value == ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(NotEqual &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value != ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(Lesser &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value < ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(LesserEq &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value <= ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(Greater &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value > ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(GreaterEq &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value >= ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(Add &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        node.val = new Bool(((Bool*)node.left->val)->value >= ((Bool*)node.left->val)->value);
    }

    void Interpreter::visit(Sub &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        if (node.left->node_type->type == INT) {
            node.val = new Int(((Int*)node.left->val)->value - ((Int*)node.left->val)->value);
        } else {
            node.val = new Float(((Float*)node.left->val)->value - ((Float*)node.left->val)->value);
        }
    }

    void Interpreter::visit(Mul &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        if (node.left->node_type->type == INT) {
            node.val = new Int(((Int*)node.left->val)->value * ((Int*)node.left->val)->value);
        } else {
            node.val = new Float(((Float*)node.left->val)->value * ((Float*)node.left->val)->value);
        }
    }

    void Interpreter::visit(Div &node)
    {
        node.left->accept(*this);
        node.right->accept(*this);

        if (node.left->node_type->type == INT) {
            node.val = new Int(((Int*)node.left->val)->value / ((Int*)node.left->val)->value);
        } else {
            node.val = new Float(((Float*)node.left->val)->value / ((Float*)node.left->val)->value);
        }
    }

    void Interpreter::visit(Mod &node)
    {
        if (node.left->node_type->type == INT) {
            node.val = new Int(((Int*)node.left->val)->value % ((Int*)node.left->val)->value);
        } else {
            //node.val = new Float(((Float*)node.left->val)->value % ((Float*)node.left->val)->value);
        }
    }

    void Interpreter::visit(ListAdd &node)
    {

    }

    void Interpreter::visit(Par &node)
    {
        node.child->accept(*this);
        node.val = node.child;
    }

    void Interpreter::visit(Not &node)
    {
        node.child->accept(*this);

        node.val = new Bool(!((Bool*)node.child->val)->value);
    }

    void Interpreter::visit(ListPattern &node)
    {

    }

    void Interpreter::visit(TuplePattern &node)
    {

    }

    void Interpreter::visit(ListSplit &node)
    {

    }

    void Interpreter::visit(Int &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(Float &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(Bool &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(Char &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(String &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(List &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(Id &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(Call &node)
    {
        node.val = &node;
    }

    void Interpreter::visit(Type &node)
    {
        //node.val = &node;
    }
}

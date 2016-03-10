#include "Interpreter.h"

using namespace common;
using namespace std;

namespace interpreter {

    Interpreter::Interpreter(Program &node) {
        this->node = node;
    }

    string Interpreter::interpret(string func_id = "main") {
        result = "";

        this->func_id = func_id;
        node->accept(this);

        return result;
    }

    void Interpreter::set_result(Expr *exp) {
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

    void Interpreter::visit(Program *node)
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

    void Interpreter::visit(Function *node)
    {
        for (auto c : node->cases) {
            if (c) {

            }
        }
    }

    void Interpreter::visit(Case *node)
    {
        /*
        for (auto e : node->expr) {
            e->accept(this);
        }
        */
    }

    void Interpreter::visit(Or *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value || ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(And *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value && ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(Equal *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value == ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(NotEqual *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value != ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(Lesser *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value < ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(LesserEq *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value <= ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(Greater *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value > ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(GreaterEq *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(((Bool*)node->left->val)->value >= ((Bool*)node->left->val)->value);
    }

    void Interpreter::visit(Add *node)
    {

    }

    void Interpreter::visit(Sub *node)
    {

    }

    void Interpreter::visit(Mul *node)
    {

    }

    void Interpreter::visit(Div *node)
    {

    }

    void Interpreter::visit(Mod *node)
    {

    }

    void Interpreter::visit(ListAdd *node)
    {

    }

    void Interpreter::visit(Par *node)
    {

    }

    void Interpreter::visit(Not *node)
    {
        node->left->accept(this);
        node->right->accept(this);

        node->val = new Bool(!((Bool*)node->child->val)->value);
    }

    void Interpreter::visit(ListPattern *node)
    {

    }

    void Interpreter::visit(TuplePattern *node)
    {

    }

    void Interpreter::visit(ListSplit *node)
    {

    }

    void Interpreter::visit(Int *node)
    {
        node->val = node;
    }

    void Interpreter::visit(Float *node)
    {
        node->val = node;
    }

    void Interpreter::visit(Bool *node)
    {
        node->val = node;
    }

    void Interpreter::visit(Char *node)
    {
        node->val = node;
    }

    void Interpreter::visit(String *node)
    {
        node->val = node;
    }

    void Interpreter::visit(List *node)
    {
        node->val = node;
    }

    void Interpreter::visit(Id *node)
    {
        node->val = node;
    }

    void Interpreter::visit(Call *node)
    {
        node->val = node;
    }

    void Interpreter::visit(Type *node)
    {
        node->val = node;
    }
}

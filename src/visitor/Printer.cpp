#include "Printer.h"
#include <iostream>

using namespace std;

namespace visitor {

    void Printer::visit(Program *node) {
        res = "";

        cout << "Program " << endl;

        for (Node *func : node->funcs) {
            func->accept(this);
        }
    }

    void Printer::visit(Function *node) {
        cout << "Function " << node->id << endl;

        res.append("def ");
        res.append(node->id);
        res.append(" : ");

        for (auto & p: node->types) {
            p->accept(this);
            if (p != node->types.back())
                res.append(" -> ");
        }

        res.append("\n");

        for (Node *_case : node->cases) {
            _case->accept(this);
            res.append("\n");
        }

        res.append("\n");
    }

    void Printer::visit(Case *node) {

        cout << "Case" << endl;

        res.append("\t| ");

        for (auto & p: node->patterns) {
            p->accept(this);
            if (p != node->patterns.back())
                res.append(" ");
        }

        res.append(" = ");

        node->expr->accept(this);
    }

    void Printer::visit(Or *node) {

        cout << "Or" << endl;

        node->left->accept(this);
        res.append(" || ");
        node->right->accept(this);
    }

    void Printer::visit(And *node) {

        cout << "And" << endl;

        node->left->accept(this);
        res.append(" && ");
        node->right->accept(this);
    }

    void Printer::visit(Equal *node) {

        cout << "Equal" << endl;

        node->left->accept(this);
        res.append(" == ");
        node->right->accept(this);
    }

    void Printer::visit(NotEqual *node) {

        cout << "Not Equal" << endl;

        node->left->accept(this);
        res.append(" != ");
        node->right->accept(this);
    }

    void Printer::visit(Lesser *node) {

        cout << "Lesser" << endl;

        node->left->accept(this);
        res.append(" < ");
        node->right->accept(this);
    }

    void Printer::visit(Greater *node) {

        cout << "Greater" << endl;

        node->left->accept(this);
        res.append(" > ");
        node->right->accept(this);
    }

    void Printer::visit(LesserEq *node) {

        cout << "LesserEq" << endl;

        node->left->accept(this);
        res.append(" <= ");
        node->right->accept(this);
    }

    void Printer::visit(GreaterEq *node) {

        cout << "GreaterEq" << endl;

        node->left->accept(this);
        res.append(" >= ");
        node->right->accept(this);
    }

    void Printer::visit(Add *node) {

        cout << "Add" << endl;

        node->left->accept(this);
        res.append(" + ");
        node->right->accept(this);
    }

    void Printer::visit(Sub *node) {

        cout << "Sub" << endl;

        node->left->accept(this);
        res.append(" - ");
        node->right->accept(this);
    }

    void Printer::visit(Mul *node) {

        cout << "Mul" << endl;

        node->left->accept(this);
        res.append(" * ");
        node->right->accept(this);
    }

    void Printer::visit(Div *node) {

        cout << "Div" << endl;

        node->left->accept(this);
        res.append(" / ");
        node->right->accept(this);
    }

    void Printer::visit(Mod *node) {

        cout << "Mod" << endl;

        node->left->accept(this);
        res.append(" % ");
        node->right->accept(this);
    }

    void Printer::visit(ListAdd *node) {

        cout << "ListAdd" << endl;

        node->left->accept(this);
        res.append(" : ");
        node->right->accept(this);
    }

    void Printer::visit(Par *node) {

        cout << "Par" << endl;

        res.append("(");
        node->child->accept(this);
        res.append(")");
    }

    void Printer::visit(Not *node) {

        cout << "Not" << endl;

        res.append("!");
        node->child->accept(this);
    }

    void Printer::visit(Int *node) {

        cout << "Int" << endl;

        res.append(to_string(node->value));
    }

    void Printer::visit(Float *node) {

        cout << "Float" << endl;

        res.append(to_string(node->value));
    }

    void Printer::visit(Bool *node) {

        cout << "Bool" << endl;

        res.append((node->value) ? "True" : "False");
    }

    void Printer::visit(Char *node) {

        cout << "Char" << endl;

        res.append("'");
        res.append(to_string(node->value));
        res.append("'");
    }

    void Printer::visit(String *node) {

        cout << "String" << endl;

        res.append("\"");
        res.append(node->value);
        res.append("\"");
    }

    void Printer::visit(List *node) {

        cout << "List" << endl;

        res.append("[");

        for (auto & p: node->exprs) {
            p->accept(this);
            if (p != node->exprs.back())
                res.append(", ");
        }

        res.append("]");
    }

    void Printer::visit(Tuple *node) {

        cout << "Tuple" << endl;

        res.append("(");

        for (auto & p: node->exprs) {
            p->accept(this);
            if (p != node->exprs.back())
                res.append(", ");
        }

        res.append(")");
    }

    void Printer::visit(Id *node) {

        cout << "Id" << endl;

        res.append(node->id);
    }

    void Printer::visit(Call *node) {

        cout << "Call " << node->exprs.size() << endl;

        node->callee->accept(this);
        res.append("(");

        for (auto & p: node->exprs) {
            p->accept(this);
            if (p != node->exprs.back())
                res.append(", ");
        }

        res.append(")");
    }

    void Printer::visit(Type *node) {

        cout << "Type" << endl;

        switch (node->type) {
            case INT:
                res.append("Int");
                break;
            case FLOAT:
                res.append("Float");
                break;
            case BOOL:
                res.append("Bool");
                break;
            case CHAR:
                res.append("Char");
                break;
            case STRING:
                res.append("String");
                break;
            case LIST:
                res.append("[");
                node->types[0]->accept(this);
                res.append("]");
                break;
            case TUPLE:
                res.append("(");
                print_collection(node, ", ");
                res.append(")");
                break;
            case SIGNATURE:
                res.append("(");
                print_collection(node, " -> ");
                res.append(")");
                break;
            default:
                break;
        }
    }

    void Printer::print_collection(Type *node, const std::string split) {

        for (auto & p: node->types) {
            p->accept(this);
            if (p != node->types.back())
                res.append(split);
        }
    }

    void Printer::visit(ListPattern *node) {

        cout << "ListPattern" << endl;

        res.append("[");

        for (auto & p: node->patterns) {
            p->accept(this);
            if (p != node->patterns.back())
                res.append(", ");
        }

        res.append("]");
    }

    void Printer::visit(TuplePattern *node) {

        cout << "TuplePattern" << endl;

        res.append("(");

        for (auto & p: node->patterns) {
            p->accept(this);
            if (p != node->patterns.back())
                res.append(", ");
        }

        res.append(")");
    }

    void Printer::visit(ListSplit *node) {

        cout << "ListSplit" << endl;

        res.append("(");
        node->left->accept(this);
        res.append(" : ");
        node->right->accept(this);
        res.append(")");
    }
}
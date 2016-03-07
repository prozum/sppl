#include "HCodeGenerator.h"
#include <iostream>
#include <string>

using namespace common;
using namespace std;

HCodeGenerator::HCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os)
{
}

void HCodeGenerator::visit(Program *node)
{
    os << "module PlaceHolder where" << endl;

    for (auto f : node->funcs) {
        f->accept(this);
    }
}

void HCodeGenerator::visit(Function *node)
{
    curr_fun = node;

    os << node->id << " :: ";

    if (node->types.size() != 0) {
        for (int i = 0; i < node->types.size() - 1; ++i) {
            node->types[i]->accept(this);
            os << " -> ";
        }

        node->types.back()->accept(this);
    }

    os << endl;

    for (auto c : node->cases) {
        c->accept(this);
    }
}

void HCodeGenerator::visit(Case *node)
{
    os << curr_fun->id << " ";

    for (auto p : node->patterns) {
        p->accept(this);
        os << " ";
    }

    os << "= ";

    node->expr->accept(this);

    os << endl;
}

void HCodeGenerator::visit(Or *node)
{
    os << "((||) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(And *node)
{
    os << "((&&) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Equal *node)
{
    os << "((==) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(NotEqual *node)
{
    os << "((/=) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Lesser *node)
{
    os << "((<) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(LesserEq *node)
{
    os << "((<=) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Greater *node)
{
    os << "((>) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(GreaterEq *node)
{
    os << "((>=) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Add *node)
{
    os << "((+) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Sub *node)
{
    os << "((-) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Mul *node)
{
    os << "((*) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Div *node)
{
    os << "((/) ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Mod *node)
{
    os << "(rem ";
    node->left->accept(this);
    os << " ";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(ListAdd *node)
{
    node->left->accept(this);
    os << ":";
    node->right->accept(this);
}

void HCodeGenerator::visit(Par *node)
{
    os << "(";
    node->child->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Not *node)
{
    os << "not ";
    node->child->accept(this);
}

void HCodeGenerator::visit(ListPattern *node)
{
    os << "[";

    if (node->patterns.size() != 0) {
        for (int i = 0; i < node->patterns.size() - 1; ++i) {
            node->patterns[i]->accept(this);
            os << ",";
        }

        node->patterns.back()->accept(this);
    }

    os << "]";
}

void HCodeGenerator::visit(TuplePattern *node)
{
    os << "(";

    if (node->patterns.size() != 0) {
        for (int i = 0; i < node->patterns.size() - 1; ++i) {
            node->patterns[i]->accept(this);
            os << ",";
        }

        node->patterns.back()->accept(this);
    }

    os << ")";
}

void HCodeGenerator::visit(ListSplit *node)
{
    os << "(";
    node->left->accept(this);
    os << ":";
    node->right->accept(this);
    os << ")";
}

void HCodeGenerator::visit(Int *node)
{
    os << node->value;
}

void HCodeGenerator::visit(Float *node)
{
    os << node->value;
}

void HCodeGenerator::visit(Bool *node)
{
    os << node->value;
}

void HCodeGenerator::visit(Char *node)
{
    os << node->value;
}

void HCodeGenerator::visit(String *node)
{
    os << node->value;
}

void HCodeGenerator::visit(List *node)
{
    os << "[";

    if (node->exprs.size() != 0) {
        for (int i = 0; i < node->exprs.size() - 1; ++i) {
            node->exprs[i]->accept(this);
            os << ",";
        }

        node->exprs.back()->accept(this);
    }

    os << "]";
}

void HCodeGenerator::visit(Tuple *node)
{
    os << "(";

    if (node->exprs.size() != 0) {
        for (int i = 0; i < node->exprs.size() - 1; ++i) {
            node->exprs[i]->accept(this);
            os << ",";
        }

        node->exprs.back()->accept(this);
    }

    os << ")";
}

void HCodeGenerator::visit(Id *node)
{
    os << node->id;
}

void HCodeGenerator::visit(Call *node)
{
    os << "(";
    node->callee->accept(this);

    os << " ";

    if (node->exprs.size() != 0) {
        for (int i = 0; i < node->exprs.size() - 1; ++i) {
            node->exprs[i]->accept(this);
            os << " ";
        }

        node->exprs.back()->accept(this);
    }
    os << ")";
}

void HCodeGenerator::visit(Type *node)
{
    if (node->types.empty()) {
        switch (node->type) {
        case INT:
            os << "Int";
            break;
        case FLOAT:
            os << "Float";
            break;
        case BOOL:
            os << "Bool";
            break;
        case CHAR:
            os << "Char";
            break;
        case STRING:
            os << "String";
        default:
            break;
        }
    } else {
        switch (node->type) {
        case LIST:
            os << "[";
            node->types.front()->accept(this);
            os << "]";
            break;
        case TUPLE:
            os << "(";
            if (node->types.size() != 0) {
                for (int i = 0; i < node->types.size() - 1; ++i) {
                    node->types[i]->accept(this);
                    os << ",";
                }

                node->types.back()->accept(this);
            }
            os << ")";
            break;
        case SIGNATURE:
            os << "(";
            if (node->types.size() != 0) {
                for (int i = 0; i < node->types.size() - 1; ++i) {
                    node->types[i]->accept(this);
                    os << " -> ";
                }

                node->types.back()->accept(this);
            }
            os << ")";
            break;
        default:
            break;
        }
    }
}

string HCodeGenerator::get_type(Type *t)
{

}

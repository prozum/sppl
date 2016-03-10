#include "HCodeGenerator.h"
#include <iostream>
#include <string>

using namespace common;
using namespace std;

namespace codegen {

    HCodeGenerator::HCodeGenerator(std::ostream &out) : CodeGenerator::CodeGenerator(out)
    {
    }

    void HCodeGenerator::visit(Program *node)
    {
        //os << "module PlaceHolder where" << endl;

        for (auto f : node->funcs) {
            if (f->id != "main") {
                f->accept(this);
            } else {
                output << "main = putStrLn (show (";
                f->cases.front()->expr->accept(this);
                output << "))\n";
            }
        }
    }

    void HCodeGenerator::visit(Function *node)
    {
        curr_fun = node;

        output << node->id << " :: ";

        if (node->types.size() != 0) {
            for (int i = 0; i < node->types.size() - 1; ++i) {
                node->types[i]->accept(this);
                output << " -> ";
            }

            node->types.back()->accept(this);
        }

        output << endl;

        for (auto c : node->cases) {
            c->accept(this);
        }

        output << endl;
    }

    void HCodeGenerator::visit(Case *node)
    {
        output << curr_fun->id << " ";

        for (auto p : node->patterns) {
            p->accept(this);
            output << " ";
        }

        output << "= ";

        node->expr->accept(this);

        output << endl;
    }

    void HCodeGenerator::visit(Or *node)
    {
        output << "((||) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(And *node)
    {
        output << "((&&) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Equal *node)
    {
        output << "((==) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(NotEqual *node)
    {
        output << "((/=) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Lesser *node)
    {
        output << "((<) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(LesserEq *node)
    {
        output << "((<=) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Greater *node)
    {
        output << "((>) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(GreaterEq *node)
    {
        output << "((>=) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Add *node)
    {
        output << "((+) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Sub *node)
    {
        output << "((-) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Mul *node)
    {
        output << "((*) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Div *node)
    {
        output << "((/) ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Mod *node)
    {
        output << "(rem ";
        node->left->accept(this);
        output << " ";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(ListAdd *node)
    {
        node->left->accept(this);
        output << ":";
        node->right->accept(this);
    }

    void HCodeGenerator::visit(Par *node)
    {
        output << "(";
        node->child->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Not *node)
    {
        output << "not ";
        node->child->accept(this);
    }

    void HCodeGenerator::visit(ListPattern *node)
    {
        output << "[";

        if (node->patterns.size() != 0) {
            for (int i = 0; i < node->patterns.size() - 1; ++i) {
                node->patterns[i]->accept(this);
                output << ",";
            }

            node->patterns.back()->accept(this);
        }

        output << "]";
    }

    void HCodeGenerator::visit(TuplePattern *node)
    {
        output << "(";

        if (node->patterns.size() != 0) {
            for (int i = 0; i < node->patterns.size() - 1; ++i) {
                node->patterns[i]->accept(this);
                output << ",";
            }

            node->patterns.back()->accept(this);
        }

        output << ")";
    }

    void HCodeGenerator::visit(ListSplit *node)
    {
        output << "(";
        node->left->accept(this);
        output << ":";
        node->right->accept(this);
        output << ")";
    }

    void HCodeGenerator::visit(Int *node)
    {
        output << node->value;
    }

    void HCodeGenerator::visit(Float *node)
    {
        output << node->value;
    }

    void HCodeGenerator::visit(Bool *node)
    {
        output << node->value;
    }

    void HCodeGenerator::visit(Char *node)
    {
        output << node->value;
    }

    void HCodeGenerator::visit(String *node)
    {
        output << node->value;
    }

    void HCodeGenerator::visit(List *node)
    {
        output << "[";

        if (node->exprs.size() != 0) {
            for (int i = 0; i < node->exprs.size() - 1; ++i) {
                node->exprs[i]->accept(this);
                output << ",";
            }

            node->exprs.back()->accept(this);
        }

        output << "]";
    }

    void HCodeGenerator::visit(Tuple *node)
    {
        output << "(";

        if (node->exprs.size() != 0) {
            for (int i = 0; i < node->exprs.size() - 1; ++i) {
                node->exprs[i]->accept(this);
                output << ",";
            }

            node->exprs.back()->accept(this);
        }

        output << ")";
    }

    void HCodeGenerator::visit(Id *node)
    {
        output << node->id;
    }

    void HCodeGenerator::visit(Call *node)
    {
        output << "(";
        node->callee->accept(this);

        output << " ";

        if (node->exprs.size() != 0) {
            for (int i = 0; i < node->exprs.size() - 1; ++i) {
                node->exprs[i]->accept(this);
                output << " ";
            }

            node->exprs.back()->accept(this);
        }
        output << ")";
    }

    void HCodeGenerator::visit(Type *node)
    {
        switch (node->type) {
        case INT:
            output << "Int";
            break;
        case FLOAT:
            output << "Float";
            break;
        case BOOL:
            output << "Bool";
            break;
        case CHAR:
            output << "Char";
            break;
        case STRING:
            output << "String";
        case LIST:
            output << "[";
            node->types.front()->accept(this);
                output << "]";
            break;
        case TUPLE:
            output << "(";
            if (node->types.size() != 0) {
                for (int i = 0; i < node->types.size() - 1; ++i) {
                    node->types[i]->accept(this);
                    output << ",";
                }

                node->types.back()->accept(this);
            }
                output << ")";
            break;
        case SIGNATURE:
            output << "(";
            if (node->types.size() != 0) {
                for (int i = 0; i < node->types.size() - 1; ++i) {
                    node->types[i]->accept(this);
                    output << " -> ";
                }

                node->types.back()->accept(this);
            }
                output << ")";
            break;
        default:
            break;
        }
    }
}

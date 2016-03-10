#include "CppCodeGenerator.h"
#include <iostream>

using namespace common;
using namespace std;

namespace codegen {

    CppCodeGenerator::CppCodeGenerator(ostream &out) : CodeGenerator::CodeGenerator(out) {
    }

    void CppCodeGenerator::visit(Program *node) {
        for (auto f : node->funcs) {
            f->accept(this);
        }
    }

    void CppCodeGenerator::visit(Function *node) {
        is_return = true;
        (node->types.back())->accept(this);
        is_return = false;

        output << " u" << node->id << "( ";

        if (node->cases.size() - 1 != 0) {
            for (int i = 0; i < node->cases.size() - 2; ++i) {
                node->types[i]->accept(this);
                output << ", ";
                arg_count++;
            }

            node->types[node->cases.size() - 2]->accept(this);
            arg_count = 0;
        }

        output << ") {" << endl;

        for (auto c : node->cases) {
            c->accept(this);
        }

        output << "}" << endl;
    }

    void CppCodeGenerator::visit(Case *node) {
        output << "if (";
        for (int i = 0; i < node->patterns.size() - 1; ++i) {
            output << " g" << i << " == ";
            node->patterns[i]->accept(this);
        }

        output << ") {" << endl;
        output << "return ";
        node->expr->accept(this);
        output << endl << "}" << endl;
    }

    void CppCodeGenerator::visit(Or *node) {
        output << "(";
        node->left->accept(this);
        output << "||";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(And *node) {
        output << "(";
        node->left->accept(this);
        output << "&&";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Equal *node) {
        output << "(";
        node->left->accept(this);
        output << "==";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(NotEqual *node) {
        output << "(";
        node->left->accept(this);
        output << "!=";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Lesser *node) {
        output << "(";
        node->left->accept(this);
        output << "<";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(LesserEq *node) {
        output << "(";
        node->left->accept(this);
        output << "<=";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Greater *node) {
        output << "(";
        node->left->accept(this);
        output << ">";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(GreaterEq *node) {
        output << "(";
        node->left->accept(this);
        output << ">=";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Add *node) {
        output << "(";
        node->left->accept(this);
        output << "+";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Sub *node) {
        output << "(";
        node->left->accept(this);
        output << "-";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Mul *node) {
        output << "(";
        node->left->accept(this);
        output << "*";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Div *node) {
        output << "(";
        node->left->accept(this);
        output << "/";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Mod *node) {
        output << "(";
        node->left->accept(this);
        output << "%";
        node->right->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(ListAdd *node) {

    }

    void CppCodeGenerator::visit(Par *node) {
        output << "(";
        node->child->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(Not *node) {
        output << "(";
        output << "!";
        node->child->accept(this);
        output << ")";
    }

    void CppCodeGenerator::visit(ListPattern *node) {

    }

    void CppCodeGenerator::visit(TuplePattern *node) {

    }

    void CppCodeGenerator::visit(ListSplit *node) {

    }

    void CppCodeGenerator::visit(Int *node) {

    }

    void CppCodeGenerator::visit(Float *node) {

    }

    void CppCodeGenerator::visit(Bool *node) {

    }

    void CppCodeGenerator::visit(Char *node) {

    }

    void CppCodeGenerator::visit(String *node) {

    }

    void CppCodeGenerator::visit(List *node) {

    }

    void CppCodeGenerator::visit(Tuple *node) {

    }

    void CppCodeGenerator::visit(Id *node) {

    }

    void CppCodeGenerator::visit(Call *node) {
        output << "(";
        node->callee->accept(this);

        output << " ";

        if (node->exprs.size() != 0) {
            for (int i = 0; i < node->exprs.size() - 1; ++i) {
                node->exprs[i]->accept(this);
                output << ",";
            }

            node->exprs.back()->accept(this);
        }
        output << ")";
    }

    void CppCodeGenerator::visit(Type *node) {
        switch (node->type) {
            case FLOAT:
                output << "double";
            case CHAR:
                output << "char";
            case INT:
                output << "int";
            case BOOL:
                output << "int";

            case STRING:
            case LIST:
            case TUPLE:
            case SIGNATURE:
            default:
                break;
        }

        if (!is_return) {
            output << " g" << arg_count;
        }
    }
}

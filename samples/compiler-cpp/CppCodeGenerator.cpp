#include "CppCodeGenerator.h"
#include <iostream>

using namespace common;
using namespace std;

int arg_count = 0;
bool is_return = false;

CCodeGenerator::CCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os)
{
}

void CCodeGenerator::visit(Program *node)
{
    for (auto f : node->funcs) {
        f->accept(this);
    }
}

void CCodeGenerator::visit(Function *node)
{
    is_return = true;
    (node->types.back())->accept(this);
    is_return = false;

    os << " u" << node->id << "( ";

    if (node->cases.size() - 1 != 0) {
        for (int i = 0; i < node->cases.size() - 2; ++i) {
            node->types[i]->accept(this);
            os << ", ";
            arg_count++;
        }

        node->types[node->cases.size() - 2]->accept(this);
        arg_count = 0;
    }

    os << ") {" << endl;

    for (auto c : node->cases) {
        c->accept(this);
    }

    os << "}" << endl;
}

void CCodeGenerator::visit(Case *node)
{
    os << "if (";
    for (int i = 0; i < node->patterns.size() - 1; ++i) {
        os << " g" << i << " == ";
        node->patterns[i]->accept(this);
    }

    os << ") {" << endl;
    os << "return ";
    node->expr->accept(this);
    os << endl << "}" << endl;
}

void CCodeGenerator::visit(Or *node)
{
    os << "(";
    node->left->accept(this);
    os << "||";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(And *node)
{
    os << "(";
    node->left->accept(this);
    os << "&&";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Equal *node)
{
    os << "(";
    node->left->accept(this);
    os << "==";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(NotEqual *node)
{
    os << "(";
    node->left->accept(this);
    os << "!=";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Lesser *node)
{
    os << "(";
    node->left->accept(this);
    os << "<";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(LesserEq *node)
{
    os << "(";
    node->left->accept(this);
    os << "<=";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Greater *node)
{
    os << "(";
    node->left->accept(this);
    os << ">";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(GreaterEq *node)
{
    os << "(";
    node->left->accept(this);
    os << ">=";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Add *node)
{
    os << "(";
    node->left->accept(this);
    os << "+";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Sub *node)
{
    os << "(";
    node->left->accept(this);
    os << "-";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Mul *node)
{
    os << "(";
    node->left->accept(this);
    os << "*";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Div *node)
{
    os << "(";
    node->left->accept(this);
    os << "/";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Mod *node)
{
    os << "(";
    node->left->accept(this);
    os << "%";
    node->right->accept(this);
    os << ")";
}

void CCodeGenerator::visit(ListAdd *node)
{
    
}

void CCodeGenerator::visit(Par *node)
{
    os << "(";
    node->child->accept(this);
    os << ")";
}

void CCodeGenerator::visit(Not *node)
{
    os << "(";
    os << "!";
    node->child->accept(this);
    os << ")";
}

void CCodeGenerator::visit(ListPattern *node)
{

}

void CCodeGenerator::visit(TuplePattern *node)
{
    
}

void CCodeGenerator::visit(ListSplit *node)
{
    
}

void CCodeGenerator::visit(Int *node)
{

}

void CCodeGenerator::visit(Float *node)
{

}

void CCodeGenerator::visit(Bool *node)
{

}

void CCodeGenerator::visit(Char *node)
{

}

void CCodeGenerator::visit(String *node)
{

}

void CCodeGenerator::visit(List *node)
{

}

void CCodeGenerator::visit(Tuple *node)
{
    
}

void CCodeGenerator::visit(Id *node)
{
    
}

void CCodeGenerator::visit(Call *node)
{
    os << "(";
    node->callee->accept(this);

    os << " ";

    if (node->exprs.size() != 0) {
        for (int i = 0; i < node->exprs.size() - 1; ++i) {
            node->exprs[i]->accept(this);
            os << ",";
        }

        node->exprs.back()->accept(this);
    }
    os << ")";
}

void CCodeGenerator::visit(Type *node)
{
    switch (node->type) {
        case FLOAT:
            os << "double";
        case CHAR:
            os << "char";
        case INT:
            os << "int";
        case BOOL:
            os << "int";

        case STRING:
        case LIST:
        case TUPLE:
        case SIGNATURE:
        default:
            break;
    }

    if (!is_return){
        os << " g" << arg_count;
    }
}

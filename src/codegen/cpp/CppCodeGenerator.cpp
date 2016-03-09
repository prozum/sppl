#include "CppCodeGenerator.h"
#include <string>
#include <iostream>

using namespace common;
using namespace std;


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

    for (auto type : node->types) {
        type->accept(this);
        arg_count++;

        if (type != node->types.back())
            os << ", ";
    }

    arg_count = 0;

    os << ") {" << endl;

    for (auto c : node->cases) {
        c->accept(this);
        real_ids.clear();
    }

    os << "}" << endl;
    arg_name.clear();
}

void CCodeGenerator::visit(Case *node)
{
    os << "if (";

    context = PATTERN;
    for (auto pattern : node->patterns){
        os << " g" << arg_count << " == ";
        pattern->accept(this);

        if (pattern != node->patterns.back())
            os << " && ";

        arg_count++;
    }

    arg_count = 0;
    context = EXPR;

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
    os << node->value;
}

void CCodeGenerator::visit(Float *node)
{
    os << node->value;
}

void CCodeGenerator::visit(Bool *node)
{
    os << node->value;
}

void CCodeGenerator::visit(Char *node)
{
    os << node->value;
}

void CCodeGenerator::visit(String *node)
{
    os << node->value;
}

void CCodeGenerator::visit(List *node)
{

}

void CCodeGenerator::visit(Tuple *node)
{
    
}

void CCodeGenerator::visit(Id *node)
{
    switch (context) {
        case PATTERN:
            real_ids.insert({node->id, arg_names[arg_count]});
            os << arg_names[arg_count];
            break;
        case EXPR:
            os << real_ids[node->id];
            break;
        default:
            throw "WOW! THIS SHOULD NEVER HAPPEN";
    }
}

void CCodeGenerator::visit(Call *node)
{
    os << "(";
    node->callee->accept(this);

    os << " ";

    for (auto expr : node->exprs){
        expr->accept(this);

        if (expr != node->exprs.back())
            os << ", ";
    }

    os << ")";
}

void CCodeGenerator::visit(Type *node)
{
    stringstream arg_name;

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
        arg_name << " g" << arg_count;
        os << arg_name.str();
        arg_names.push_back(arg_name.str());
    }
}

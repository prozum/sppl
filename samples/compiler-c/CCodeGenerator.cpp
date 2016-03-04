#include "CCodeGenerator.h"
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

string CCodeGenerator::getType(types t)
{
    switch (t) {
    case INT:
        return "int64_t";
    case FLOAT:
        return "double";
    case BOOL:
        return "char";
    case CHAR:
        return "char";
    default:
        return "char*";
    }
}

void CCodeGenerator::visit(Function *node)
{
    if (node->id == "main") {
        os << "void" << " main() {" << endl;

        for (auto c : node->cases) {
            c->accept(this);
        }

        os << "}" << endl;
    } else {
        os << "INSERT WHATEVER TYPE HERE" << endl;

        for (auto c : node->cases) {
            c->accept(this);
        }

        os << "}" << endl;
    }
}

void CCodeGenerator::visit(Case *node)
{

}

void CCodeGenerator::visit(Or *node)
{
    
}

void CCodeGenerator::visit(And *node)
{
    
}

void CCodeGenerator::visit(Equal *node)
{
    
}

void CCodeGenerator::visit(NotEqual *node)
{
    
}

void CCodeGenerator::visit(Lesser *node)
{
    
}

void CCodeGenerator::visit(LesserEq *node)
{

}

void CCodeGenerator::visit(Greater *node)
{

}

void CCodeGenerator::visit(GreaterEq *node)
{
    
}

void CCodeGenerator::visit(Add *node)
{
    
}

void CCodeGenerator::visit(Sub *node)
{
    
}

void CCodeGenerator::visit(Mul *node)
{
    
}

void CCodeGenerator::visit(Div *node)
{
    
}

void CCodeGenerator::visit(Mod *node)
{
    
}

void CCodeGenerator::visit(ListAdd *node)
{
    
}

void CCodeGenerator::visit(Par *node)
{
    
}

void CCodeGenerator::visit(Not *node)
{
    
}

void CCodeGenerator::visit(Type *node)
{

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
    
}

void CCodeGenerator::visit(LiteralType *node)
{

}

{
    
}

void CCodeGenerator::visit(ListType *node)
{
    
}

void CCodeGenerator::visit(TupleType *node)
{
    
}

void CCodeGenerator::visit(Signature *node)
{

}

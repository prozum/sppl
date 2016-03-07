#include "CppCodeGenerator.h"
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
    if (node->id == "main") {
        os << "void" << " main() {" << endl;

        for (auto c : node->cases) {
            c->accept(this);
        }

        os << "}" << endl;
    } else {
        (node->types.back())->accept(this);

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
    
}

void CCodeGenerator::visit(Type *node)
{

}

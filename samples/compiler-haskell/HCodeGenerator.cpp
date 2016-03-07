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
    for (auto f : node->funcs) {
        f->accept(this);
    }
}

void HCodeGenerator::visit(Function *node)
{
    os << node->id << " :: ";


}

void HCodeGenerator::visit(Case *node)
{

}

void HCodeGenerator::visit(Or *node)
{
    
}

void HCodeGenerator::visit(And *node)
{
    
}

void HCodeGenerator::visit(Equal *node)
{
    
}

void HCodeGenerator::visit(NotEqual *node)
{
    
}

void HCodeGenerator::visit(Lesser *node)
{
    
}

void HCodeGenerator::visit(LesserEq *node)
{

}

void HCodeGenerator::visit(Greater *node)
{

}

void HCodeGenerator::visit(GreaterEq *node)
{
    
}

void HCodeGenerator::visit(Add *node)
{
    
}

void HCodeGenerator::visit(Sub *node)
{
    
}

void HCodeGenerator::visit(Mul *node)
{
    
}

void HCodeGenerator::visit(Div *node)
{
    
}

void HCodeGenerator::visit(Mod *node)
{
    
}

void HCodeGenerator::visit(ListAdd *node)
{
    
}

void HCodeGenerator::visit(Par *node)
{
    
}

void HCodeGenerator::visit(Not *node)
{
    
}

void HCodeGenerator::visit(ListPattern *node)
{
    
}

void HCodeGenerator::visit(TuplePattern *node)
{
    
}

void HCodeGenerator::visit(ListSplit *node)
{
    
}

void HCodeGenerator::visit(Int *node)
{

}

void HCodeGenerator::visit(Float *node)
{

}

void HCodeGenerator::visit(Bool *node)
{

}

void HCodeGenerator::visit(Char *node)
{

}

void HCodeGenerator::visit(String *node)
{

}

void HCodeGenerator::visit(List *node)
{
    
}

void HCodeGenerator::visit(Tuple *node)
{
    
}

void HCodeGenerator::visit(Id *node)
{
    
}

void HCodeGenerator::visit(Call *node)
{
    
}

void HCodeGenerator::visit(Type *node)
{
    if (node->types.empty()) {
        //os <<
    } else {

    }
}

string HCodeGenerator::get_type(TypeEnum t)
{

}

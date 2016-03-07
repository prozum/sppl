#include "GasCodeGenerator.h"
#include <iostream>
#include <vector>

using namespace common;
using namespace std;

std::string function;

vector<string> funcVector;
vector<string> funcGlobl;

int caseCount = 0;

GasCodeGenerator::GasCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os) {
}

void GasCodeGenerator::visit(Program *node) {
    // Visit all functions
    for (auto func : node->funcs) {
        func->accept(this);
    }

    // Print all functions to output stream
    for (auto s : funcVector) {
        std::cout << s << std::endl;
    }
}

void GasCodeGenerator::visit(Function *node) {
    string funcName = node->id;
    funcGlobl.push_back(funcName);
    function.append(funcName);
    function.append(":\n");
    function.append("pushl %ebp\n");
    function.append("movl %esp, %ebp\n");
    function.append(".");
    function.append(funcName);
    function.append("funcstart:\n");

    caseCount = 0;

    for (auto funcCase : node->cases) {
        funcCase->accept(this);
    }

    function.append(".");
    function.append(funcName);
    function.append("funcend:\n");
    function.append("movl %ebp, %esp\n");
    function.append("popl %ebp\n");
    function.append("leave\n");
    function.append("ret\n");

    funcVector.push_back(function);

    function.clear();
}

void GasCodeGenerator::visit(Case *node) {
    caseCount++;
    function.append(".case");
    function.append(to_string(caseCount));
    function.append(":\n");
}

void GasCodeGenerator::visit(Or *node) {

}

void GasCodeGenerator::visit(And *node) {

}

void GasCodeGenerator::visit(Equal *node) {

}

void GasCodeGenerator::visit(NotEqual *node) {

}

void GasCodeGenerator::visit(Lesser *node) {

}

void GasCodeGenerator::visit(Greater *node) {

}

void GasCodeGenerator::visit(LesserEq *node) {

}

void GasCodeGenerator::visit(GreaterEq *node) {

}

void GasCodeGenerator::visit(Add *node) {

}

void GasCodeGenerator::visit(Sub *node) {

}

void GasCodeGenerator::visit(Mul *node) {

}

void GasCodeGenerator::visit(Div *node) {

}

void GasCodeGenerator::visit(Mod *node) {

}

void GasCodeGenerator::visit(ListAdd *node) {

}

void GasCodeGenerator::visit(Par *node) {

}

void GasCodeGenerator::visit(Not *node) {

}

void GasCodeGenerator::visit(Int *node) {

}

void GasCodeGenerator::visit(Float *node) {

}

void GasCodeGenerator::visit(Bool *node) {

}

void GasCodeGenerator::visit(Char *node) {

}

void GasCodeGenerator::visit(String *node) {

}

void GasCodeGenerator::visit(ListPattern *node) {

}

void GasCodeGenerator::visit(TuplePattern *node) {

}

void GasCodeGenerator::visit(ListSplit *node) {

}

void GasCodeGenerator::visit(List *node) {

}

void GasCodeGenerator::visit(Tuple *node) {

}

void GasCodeGenerator::visit(Id *node) {

}

void GasCodeGenerator::visit(Call *node) {

}

void GasCodeGenerator::visit(Type *node) {

}

string GasCodeGenerator::get_type(Type *) {

}
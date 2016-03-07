#include "GasCodeGenerator.h"

using namespace common;
using namespace std;

GasCodeGenerator::GasCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os) {
}

void GasCodeGenerator::visit(Program *node) {

}

void GasCodeGenerator::visit(Function *node) {

}

void GasCodeGenerator::visit(Case *node) {

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
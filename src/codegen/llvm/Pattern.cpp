#include "LLVMCodeGenerator.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::FloatPattern &Node) {
    CurVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
}

void LLVMCodeGenerator::visit(common::IntPattern &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::BoolPattern &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::CharPattern &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::IdPattern &Node) {
    ContextValues[Node.Val] = CurVal;
}
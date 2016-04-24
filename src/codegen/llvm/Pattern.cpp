#include "LLVMCodeGenerator.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::FloatPattern &Node) {
    auto PatVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
    CurVal = Builder.CreateFCmpONE(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGenerator::visit(common::IntPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGenerator::visit(common::BoolPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGenerator::visit(common::CharPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGenerator::visit(common::IdPattern &Node) {
    ContextValues[Node.Val] = CurVal;
}
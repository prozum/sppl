#include "LLVMCodeGenerator.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::FloatPattern &Node) {
    auto PatVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
    CurVal = Builder.CreateFCmpONE(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::IntPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::BoolPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::CharPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::IdPattern &Node) {
    CtxVals[Node.Val] = *CurArg;
    CurVal = ConstantInt::get(llvm::Type::getInt1Ty(getGlobalContext()), 1);
}
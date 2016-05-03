#include "LLVMCodeGen.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGen::visit(common::FloatPattern &Node) {
    auto PatVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
    CurVal = Builder.CreateFCmpONE(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGen::visit(common::IntPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGen::visit(common::BoolPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGen::visit(common::CharPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
}

void LLVMCodeGen::visit(common::IdPattern &Node) {
    CtxVals[Node.Val] = *CurArg;
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGen::visit(common::WildPattern &Node) {
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGen::visit(common::TuplePattern &Node) {
    std::vector<llvm::Constant *> TupleVal;
    for (auto &Element : Node.Patterns) {
        Element->accept(*this);
        TupleVal.push_back(static_cast<Constant *>(CurVal));
    }

    auto ConstVal = ConstantStruct::get(getTupleType(Node.RetTy), TupleVal);
    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGen::visit(common::ListPattern &Node) {
    std::vector<llvm::Constant *> ListData;

    for (auto &Element : Node.Patterns) {
        Element->accept(*this);
        ListData.push_back(static_cast<Constant *>(CurVal));
    }

    auto ListType = ArrayType::get(getType(Node.RetTy), Node.Patterns.size());
    auto ConstVal = ConstantArray::get(ListType, ListData);

    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGen::visit(common::ParPattern &Node) {
    Node.Pat->accept(*this);
}

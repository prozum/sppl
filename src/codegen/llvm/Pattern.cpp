#include "LLVMCodeGenerator.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::FloatPattern &Node) {
    auto PatVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
    CurVal = Builder.CreateFCmpONE(PatVal, &*CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::IntPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, &*CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::BoolPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, &*CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::CharPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, &*CurArg, "cmptmp");
}

void LLVMCodeGenerator::visit(common::IdPattern &Node) {
    CtxVals[Node.Val] = &*CurArg;
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGenerator::visit(common::WildPattern &Node) {
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGenerator::visit(common::TuplePattern &Node) {
    std::vector<llvm::Constant *> TupleVal;
    for (auto &Element : Node.Patterns) {
        Element->accept(*this);
        TupleVal.push_back((Constant *)CurVal);
    }

    auto ConstVal = ConstantStruct::get(getTupleType(Node.RetTy), TupleVal);
    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGenerator::visit(common::ListPattern &Node) {
    std::vector<llvm::Constant *> ListData;

    for (auto &Element : Node.Patterns) {
        Element->accept(*this);
        ListData.push_back(dynamic_cast<Constant *>(CurVal));
    }

    auto ListType = ArrayType::get(getType(Node.RetTy), Node.Patterns.size());
    auto ConstVal = ConstantArray::get(ListType, ListData);

    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGenerator::visit(common::ParPattern &Node) {
    Node.Pat->accept(*this);
}

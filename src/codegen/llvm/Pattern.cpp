#include "LLVMCodeGen.h"

using namespace llvm;
using namespace codegen;
using namespace std;

void LLVMCodeGen::visit(common::FloatPattern &Node) {
    auto PatVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
    CurVal = Builder.CreateFCmpONE(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGen::visit(common::IntPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGen::visit(common::BoolPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGen::visit(common::CharPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, CurVal, "cmptmp");
}

void LLVMCodeGen::visit(common::IdPattern &Node) {
    PatVals[Node.Val] = CurVal;
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGen::visit(common::WildPattern &Node) {
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGen::visit(common::TuplePattern &Node) {
    auto TupVal = CurVal;
    auto PatId = 0;

    addPrefix("tuple");
    auto Block = BasicBlock::Create(Ctx, getPrefix(), CurFunc);
    Builder.CreateBr(Block);
    for (auto Pat = Node.Patterns.cbegin(); Pat != Node.Patterns.cend(); ++Pat) {
        Builder.SetInsertPoint(Block);

        CurVal = Builder.CreateStructGEP(TupVal->getType()->getPointerElementType(), TupVal, PatId++);
        CurVal = Builder.CreateLoad(CurVal->getType()->getPointerElementType(), CurVal, "loadtmp");
        (*Pat)->accept(*this);
        stepPrefix();

        if (next(Pat) != Node.Patterns.cend()) {
            Block = BasicBlock::Create(Ctx, getPrefix(), CurFunc);
            Builder.CreateCondBr(CurVal, Block, NextCaseBlock);
        }
    }
    delPrefix();

    CurPatBlock = Block;
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

void LLVMCodeGen::visit(common::ListSplit &Node) {
    auto CurNode = CurVal;
    CurVal = Builder.CreateStructGEP(CurVal->getType()->getPointerElementType(), CurVal, 0);
    CurVal = Builder.CreateLoad(CurVal);
    Node.Left->accept(*this);

    CurPatBlock = BasicBlock::Create(Ctx, CurPatBlock->getName() + "_split", CurFunc);
    Builder.CreateCondBr(CurVal, CurPatBlock, NextCaseBlock);
    Builder.SetInsertPoint(CurPatBlock);
    
    CurVal = Builder.CreateStructGEP(CurNode->getType()->getPointerElementType(), CurNode, 1);
    CurVal = Builder.CreateLoad(CurVal);
    Node.Right->accept(*this);
}

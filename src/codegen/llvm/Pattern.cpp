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
            Builder.CreateCondBr(CurVal, Block, NextBlock);
        }
    }
    delPrefix();

    CurPatBlock = Block;
}

void LLVMCodeGen::visit(common::ListPattern &Node) {
    auto ListVal = CurVal;

    addPrefix("list");
    auto NullBlock = BasicBlock::Create(Ctx, getPrefix("null"), CurFunc);
    auto ListBlock = BasicBlock::Create(Ctx, getPrefix(), CurFunc);
    Builder.CreateBr(NullBlock);
    Builder.SetInsertPoint(NullBlock);

    for (auto Element = Node.Patterns.cbegin(); Element < Node.Patterns.cend(); ++Element) {
        // Check if empty node
        CurVal = ConstantPointerNull::get(static_cast<PointerType *>(ListVal->getType()));
        CurVal = Builder.CreateICmpNE(ListVal, CurVal);
        Builder.CreateCondBr(CurVal, ListBlock, NextBlock);

        // Check element
        Builder.SetInsertPoint(ListBlock);
        CurVal = Builder.CreateStructGEP(ListVal->getType()->getPointerElementType(), ListVal, 0);
        CurVal = Builder.CreateLoad(CurVal->getType()->getPointerElementType(), CurVal, "loadtmp");
        (*Element)->accept(*this);

        if (next(Element) != Node.Patterns.cend()) {
            stepPrefix();
            NullBlock = BasicBlock::Create(Ctx, getPrefix("null"), CurFunc);
            ListBlock = BasicBlock::Create(Ctx, getPrefix(), CurFunc);
            Builder.CreateCondBr(CurVal, NullBlock, NextBlock);

            Builder.SetInsertPoint(NullBlock);
            CurVal = Builder.CreateStructGEP(ListVal->getType()->getPointerElementType(), ListVal, 1);
            ListVal = Builder.CreateLoad(CurVal->getType()->getPointerElementType(), CurVal, "loadtmp");
        }

    }

    auto EndBlock = BasicBlock::Create(Ctx, getPrefix("end"), CurFunc);
    Builder.CreateCondBr(CurVal, EndBlock, NextBlock);

    // Check if list ends
    Builder.SetInsertPoint(EndBlock);
    CurVal = Builder.CreateStructGEP(ListVal->getType()->getPointerElementType(), ListVal, 1);
    ListVal = Builder.CreateLoad(CurVal->getType()->getPointerElementType(), CurVal, "loadtmp");
    CurVal = ConstantPointerNull::get(static_cast<PointerType *>(ListVal->getType()));
    CurVal = Builder.CreateICmpEQ(ListVal, CurVal);
    delPrefix();
}

void LLVMCodeGen::visit(common::ParPattern &Node) {
    Node.Pat->accept(*this);
}

void LLVMCodeGen::visit(common::ListSplit &Node) {
    auto ListVal = CurVal;

    addPrefix("split", false);

    // Check if empty list
    CurPatBlock = BasicBlock::Create(Ctx, getPrefix("null"), CurFunc);
    CurVal = ConstantPointerNull::get(static_cast<PointerType *>(ListVal->getType()));
    CurVal = Builder.CreateICmpNE(ListVal, CurVal);
    Builder.CreateCondBr(CurVal, CurPatBlock, NextBlock);
    Builder.SetInsertPoint(CurPatBlock);

    // Check element
    CurVal = Builder.CreateStructGEP(ListVal->getType()->getPointerElementType(), ListVal, 0);
    CurVal = Builder.CreateLoad(CurVal);
    Node.Left->accept(*this);

    // Split list
    CurPatBlock = BasicBlock::Create(Ctx, getPrefix(), CurFunc);
    Builder.CreateCondBr(CurVal, CurPatBlock, NextBlock);
    Builder.SetInsertPoint(CurPatBlock);

    CurVal = Builder.CreateStructGEP(ListVal->getType()->getPointerElementType(), ListVal, 1);
    CurVal = Builder.CreateLoad(CurVal);
    Node.Right->accept(*this);
    delPrefix();
}

#include "LLVMCodeGen.h"

using namespace llvm;
using namespace codegen;
using namespace std;

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
    PatVals[Node.Val] = CurVal;
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGen::visit(common::WildPattern &Node) {
    CurVal = ConstantInt::get(Int1, 1);
}

void LLVMCodeGen::visit(common::TuplePattern &Node) {

    auto TupVal = CurVal;
    auto PatId = 0;

    auto Block = BasicBlock::Create(Ctx, "tuple", CurFunc);
    Builder.CreateBr(Block);
    for (auto Pat = Node.Patterns.cbegin(); Pat != Node.Patterns.cend(); ++Pat) {
        Builder.SetInsertPoint(Block);

        CurVal = Builder.CreateStructGEP(TupVal->getType()->getPointerElementType(), TupVal, PatId++);
        CurVal = Builder.CreateLoad(CurVal->getType()->getPointerElementType(), CurVal, "loadtmp");
        (*Pat)->accept(*this);

        if (next(Pat) != Node.Patterns.cend()) {
            Block = BasicBlock::Create(Ctx, "tuple", CurFunc);
            Builder.CreateCondBr(CurVal, Block, FalseBlock);
        }
    }

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

    /*
    CurArg;
    CurVal = Builder.CreateStructGEP((*CurArg)->getType()->getPointerElementType(), *CurArg, 0);
    static_cast<common::IdPattern *>(Node.Right.get())->Val;
    auto Element =
     */
}

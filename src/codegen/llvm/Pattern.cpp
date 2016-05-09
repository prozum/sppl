#include "LLVMCodeGen.h"

using namespace llvm;
using namespace codegen;
using namespace std;

void LLVMCodeGen::visit(common::FloatPattern &Node) {
    auto PatVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
    CurVal = Builder.CreateFCmpONE(PatVal, *CurArg, "cmptmp");
    Builder.CreateStore(CurVal, PatBool);
}

void LLVMCodeGen::visit(common::IntPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
    Builder.CreateStore(CurVal, PatBool);
}

void LLVMCodeGen::visit(common::BoolPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
    Builder.CreateStore(CurVal, PatBool);
}

void LLVMCodeGen::visit(common::CharPattern &Node) {
    auto PatVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
    CurVal = Builder.CreateICmpEQ(PatVal, *CurArg, "cmptmp");
    Builder.CreateStore(CurVal, PatBool);
}

void LLVMCodeGen::visit(common::IdPattern &Node) {
    PatVals[Node.Val] = CurVal;
    CurVal = ConstantInt::get(Int1, 1);
    Builder.CreateStore(CurVal, PatBool);
}

void LLVMCodeGen::visit(common::WildPattern &Node) {
    CurVal = ConstantInt::get(Int1, 1);
    Builder.CreateStore(CurVal, PatBool);
}

void LLVMCodeGen::visit(common::TuplePattern &Node) {

    // Create block for each tuple element
    vector<BasicBlock *> Blocks;
    for (size_t i = 0; i < Node.Patterns.size(); ++i) {
        Blocks.push_back(BasicBlock::Create(Ctx, (*PatBlock)->getName() + "_tuple", CurFunc));
    }

    // Br to first tuple
    Builder.CreateBr(Blocks.front());

    auto PatId = 0;
    auto Block = Blocks.cbegin();
    auto TupVal = CurVal;
    for (auto Pat = Node.Patterns.cbegin();
         Pat != Node.Patterns.cend();
         ++Pat, ++Block) {
        Builder.SetInsertPoint(*Block);

        CurVal = Builder.CreateStructGEP(TupVal->getType()->getPointerElementType(), TupVal, PatId++);
        CurVal = Builder.CreateLoad(CurVal->getType()->getPointerElementType(), CurVal, "loadtmp");
        (*Pat)->accept(*this);

        CurVal = Builder.CreateLoad(Int1, PatBool, "tmpload");
        if (next(Block) != Blocks.cend())
            Builder.CreateCondBr(CurVal, *next(Block), PatRetBlock);
        //else
        //    Builder.CreateCondBr(CurVal, )
    }
    //Builder.CreateBr(CurPatPhiBlock);
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

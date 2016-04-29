#include "LLVMCodeGenerator.h"

using namespace std;
using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::Function &Node) {
    // Create function and entry block
    CurFunc = llvm::Function::Create(getFuncType(Node.Signature),
                                     llvm::Function::ExternalLinkage, Node.Id,
                                     Module.get());
    CurEntry = BasicBlock::Create(Ctx, "entry", CurFunc);

    // Create error block
    CurErrBlock = BasicBlock::Create(Ctx, "error", CurFunc);
    Builder.SetInsertPoint(CurErrBlock);
    Builder.CreateRet(UndefValue::get(CurFunc->getReturnType()));

    // Setup return block and phi node
    CurRetBlock = BasicBlock::Create(Ctx, "ret", CurFunc);
    Builder.SetInsertPoint(CurRetBlock);

    // Create ret instruction
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID) {
        CurPhiNode = Builder.CreatePHI(CurFunc->getReturnType(),
                                       (unsigned) Node.Cases.size(), "rettmp");
        Builder.CreateRet(CurPhiNode);
    } else {
        Builder.CreateRetVoid();
    }

    // Setup names for arguments
    auto ArgId = 0;
    for (auto &Arg : CurFunc->args()) {
        Arg.setName("_arg" + to_string(ArgId++));
    }

    // Setup case and pattern blocks
    CaseBlocks.clear();
    PatVecBlocks.clear();
    for (size_t i = 0; i < Node.Cases.size(); ++i) {
        auto PatVecBlock = vector<BasicBlock *>();
        for (size_t j = 0; j < Node.Cases[i]->Patterns.size(); ++j) {
            PatVecBlock.push_back(BasicBlock::Create(
                Ctx,
                "case" + to_string(i) + "_pattern" + to_string(j),
                CurFunc));
        }
        PatVecBlocks.push_back(PatVecBlock);

        CaseBlocks.push_back(BasicBlock::Create(
            Ctx, "case" + to_string(i), CurFunc));
    }

    // Visit cases
    for (CurCase = Node.Cases.cbegin(), CurCaseBlock = CaseBlocks.cbegin(),
        LastCaseBlock = CaseBlocks.cend(),
        CurPatVecBlock = PatVecBlocks.cbegin();
         CurCase != Node.Cases.cend();
         ++CurCase, ++CurCaseBlock, ++CurPatVecBlock) {
        (*CurCase)->accept(*this);
    }

    // Make entry point to first block
    Builder.SetInsertPoint(CurEntry);
    BasicBlock *FirstBlock;
    if (PatVecBlocks.front().empty())
        FirstBlock = CaseBlocks.front();
    else
        FirstBlock = PatVecBlocks.front().front();
    Builder.CreateBr(FirstBlock);

    verifyFunction(*CurFunc);
}

void LLVMCodeGenerator::visit(common::Case &Node) {

    BasicBlock *TrueBlock;
    BasicBlock *FalseBlock;

    CtxVals.clear();
    for (CurPat = Node.Patterns.cbegin(), CurArg = CurFunc->arg_begin(),
        CurPatBlock = CurPatVecBlock->cbegin(),
        LastPatBlock = CurPatVecBlock->cend();
         CurPat != Node.Patterns.cend(); ++CurPat, ++CurArg, ++CurPatBlock) {

        if (next(CurPatBlock) != LastPatBlock)
            TrueBlock = *next(CurPatBlock);
        else
            TrueBlock = *CurCaseBlock;

        if (next(CurCaseBlock) != LastCaseBlock)
            FalseBlock = (*next(CurPatVecBlock)).front();
        else
            FalseBlock = CurErrBlock;


        Builder.SetInsertPoint(*CurPatBlock);
        (*CurPat)->accept(*this);

        // Create condition
        Builder.CreateCondBr(CurVal, TrueBlock, FalseBlock);
    }

    // Generate expression in case block
    Builder.SetInsertPoint(*CurCaseBlock);
    Node.Expr->accept(*this);

    //auto Ty1 = CurPhiNode->getType();
    //auto Ty2 = CurVal->getType();

    //auto Arr1 = CurVal->getType()->getPointerElementType()->getArrayNumElements();
    //auto Arr2 = CurPhiNode->getType()->getPointerElementType()->getArrayNumElements();


    // Ignore expression type if function is void
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID)
        CurPhiNode->addIncoming(CurVal, *CurCaseBlock);

    // Add return value to phi node
    Builder.CreateBr(CurRetBlock);
}

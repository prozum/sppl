#include "LLVMCodeGenerator.h"

using namespace llvm;
using namespace codegen;

void LLVMCodeGenerator::visit(common::Function &Node) {
    // Create function and entry block
    CurFunc = llvm::Function::Create(getFuncType(Node.Signature), llvm::Function::ExternalLinkage, Node.Id, Module.get());
    CurEntry = BasicBlock::Create(getGlobalContext(), "entry", CurFunc);

    // Create error block
    CurErrBlock = BasicBlock::Create(getGlobalContext(), "error", CurFunc);
    Builder.SetInsertPoint(CurErrBlock);
    Builder.CreateRet(UndefValue::get(CurFunc->getReturnType()));

    // Setup return block and phi node
    CurRetBlock = BasicBlock::Create(getGlobalContext(), "ret", CurFunc);
    Builder.SetInsertPoint(CurRetBlock);
    CurPhiNode = Builder.CreatePHI(CurFunc->getReturnType(), (unsigned)Node.Cases.size(), "rettmp");
    Builder.CreateRet(CurPhiNode);

    // Setup names for arguments
    auto ArgId = 0;
    for (auto &Arg : CurFunc->args()) {
        Arg.setName("_arg" + to_string(ArgId++));
        Args.push_back(&Arg);
    }

    // Setup case and pattern blocks
    CaseBlocks.clear();
    PatVecBlocks.clear();
    int PatId, CaseId = 0;
    for (auto &Case : Node.Cases) {
        PatId = 0;
        auto PatVecBlock = vector<BasicBlock *>();
        for (auto &Pattern : Case->Patterns) {
            PatVecBlock.push_back(BasicBlock::Create(getGlobalContext(), "case" + to_string(CaseId) + "_pattern" + to_string(PatId++), CurFunc));
        }
        PatVecBlocks.push_back(PatVecBlock);

        CaseBlocks.push_back(BasicBlock::Create(getGlobalContext(), "case" + to_string(CaseId++), CurFunc));
    }

    // Visit cases
    for (CurCase = Node.Cases.cbegin(), CurCaseBlock = CaseBlocks.cbegin(), LastCaseBlock = CaseBlocks.cend(),
         CurPatVecBlock = PatVecBlocks.cbegin();
         CurCase != Node.Cases.cend();
         ++CurCase, ++CurCaseBlock, ++CurPatVecBlock) {
         (*CurCase)->accept(*this);
    }

    // Make entry point to first block
    Builder.SetInsertPoint(CurEntry);
    BasicBlock * FirstBlock;
    if (PatVecBlocks[0].empty())
        FirstBlock = CaseBlocks[0];
    else
        FirstBlock = PatVecBlocks[0][0];
    Builder.CreateBr(FirstBlock);

    verifyFunction(*CurFunc);
}

void LLVMCodeGenerator::visit(common::Case &Node) {

    BasicBlock *TrueBlock;
    BasicBlock *FalseBlock;

    CtxVals.clear();
    for (CurPat = Node.Patterns.cbegin(), CurArg = Args.cbegin(),
         CurPatBlock = CurPatVecBlock->cbegin(), LastPatBlock = CurPatVecBlock->cend();
         CurPat != Node.Patterns.cend();
         ++CurPat, ++CurArg, ++CurPatBlock) {




        if (next(CurPatBlock) != LastPatBlock)
            TrueBlock = *next(CurPatBlock);
        else
            TrueBlock = *CurCaseBlock;


        if (next(CurCaseBlock) != LastCaseBlock)
            FalseBlock = (*next(CurPatVecBlock))[0];
        else
            FalseBlock = CurErrBlock;


        Builder.SetInsertPoint(*CurPatBlock);
        (*CurPat)->accept(*this);

        // Create condition
        Builder.CreateCondBr(CurVal, TrueBlock, FalseBlock);
    }

    if (Node.Patterns.size() == 0) {
        //auto PatBlock = BasicBlock::Create(getGlobalContext(),
        //                                     "case" + to_string(CurCaseId) + "_pattern" + to_string(0),
        //                                     CurFunc);
        //Builder.SetInsertPoint(CurEntry);
        //Builder.CreateBr(*CurCaseBlock);
    }

    // Generate expression in case block
    Builder.SetInsertPoint(*CurCaseBlock);
    Node.Expr->accept(*this);


    //auto Ty1 = CurPhiNode->getType();
    //auto Ty2 = CurVal->getType();

    // Add return value to phi node
    CurPhiNode->addIncoming(CurVal, *CurCaseBlock);
    Builder.CreateBr(CurRetBlock);
}


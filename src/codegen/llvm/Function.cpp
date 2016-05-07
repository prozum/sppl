#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;

void LLVMCodeGen::visit(common::Function &Node) {
    // Create function and entry block
    Args.clear();
    if (Node.Id == string("main")) {
        CurFunc = CreateMain();
    }
    else {
        CurFunc = llvm::Function::Create(getFuncType(Node.Signature),
                                         llvm::Function::ExternalLinkage, Node.Id,
                                         Module.get());
        // Setup names for arguments
        auto ArgId = 0;
        for (auto &Arg : CurFunc->args()) {
            Arg.setName("_arg" + to_string(ArgId++));
            Args.push_back(&Arg);
        }

        CurEntry = BasicBlock::Create(Ctx, "entry", CurFunc);
    }

    // Set call convention to fast to enable tail recursion optimizations
    CurFunc->setCallingConv(CallingConv::Fast);

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

void LLVMCodeGen::visit(common::Case &Node) {

    BasicBlock *TrueBlock;
    BasicBlock *FalseBlock;

    // Store tail recursion state in visitor
    TailRec = Node.TailRec;

    CtxVals.clear();
    for (CurPat = Node.Patterns.cbegin(), CurArg = Args.begin(),
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

    // Ignore expression type if function is void
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID)
        CurPhiNode->addIncoming(CurVal, *CurCaseBlock);

    // Add return value to phi node
    Builder.CreateBr(CurRetBlock);
}

llvm::Function *LLVMCodeGen::CreateMain()
{
    // Input Type
    auto Type = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});

    // Create Function
    auto Func = llvm::Function::Create(MainType,
                                       llvm::Function::ExternalLinkage, "main",
                                       Module.get());

    // System arguments
    auto ArgIter = Func->args().begin();
    Argument *Argc = &*(ArgIter++);
    Argument *Argv = &*(ArgIter);

    // Create blocks
    CurEntry = BasicBlock::Create(Ctx, "entry", Func);
    auto CheckBlock = BasicBlock::Create(Ctx, "check", Func);
    auto UpdateBlock = BasicBlock::Create(Ctx, "update", Func);
    auto LoopBlock = BasicBlock::Create(Ctx, "loop", Func);
    auto EndBlock = BasicBlock::Create(Ctx, "end", Func);

    // Init in entry block
    Builder.SetInsertPoint(CurEntry);
    auto Iter = Builder.CreateAlloca(Int32);
    Builder.CreateStore(ConstantInt::get(Int32, APInt(32, 0)), Iter);

    // Data
    CurVal = ConstantInt::get(Int32, APInt(32, DataLayout.getPointerSize()));
    CurVal = Builder.CreateMul(Argc, CurVal);
    auto Data = CreateMalloc(CurVal, CurEntry);

    // List
    auto List = CreateList(Type, Data, Argc, CurEntry);
    Builder.CreateBr(CheckBlock);

    // Check block
    Builder.SetInsertPoint(CheckBlock);
    CurVal = Builder.CreateLoad(Int32, Iter, "loadtmp");
    CurVal = Builder.CreateICmpULT(CurVal, Argc, "cmptmp");
    Builder.CreateCondBr(CurVal, LoopBlock, EndBlock);

    // Update block
    Builder.SetInsertPoint(UpdateBlock);
    CurVal = Builder.CreateLoad(Int32, Iter, "loadtmp");
    CurVal = Builder.CreateAdd(CurVal, ConstantInt::get(Int32, APInt(32, 1)), "addtmp");
    Builder.CreateStore(CurVal, Iter);
    Builder.CreateBr(CheckBlock);

    // Loop block
    //CreateMalloc()


    // End block
    // TODO

    Args.push_back(List);

    return Func;
}

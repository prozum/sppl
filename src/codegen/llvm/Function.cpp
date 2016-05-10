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

        Entry = BasicBlock::Create(Ctx, "entry", CurFunc);
    }

    // Allocate bool for pattern matching
    Builder.SetInsertPoint(Entry);
    PatBool = Builder.CreateAlloca(Int1);

    // Set call convention to fast to enable tail recursion optimizations
    CurFunc->setCallingConv(CallingConv::Fast);

    // Create error block
    ErrBlock = BasicBlock::Create(Ctx, "error", CurFunc);
    Builder.SetInsertPoint(ErrBlock);
    Builder.CreateRet(UndefValue::get(CurFunc->getReturnType()));

    // Setup return block and phi node
    CaseRetBlock = BasicBlock::Create(Ctx, "ret", CurFunc);
    Builder.SetInsertPoint(CaseRetBlock);

    // Create ret instruction
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID) {
        CasePhiNode = Builder.CreatePHI(CurFunc->getReturnType(),
                                       (unsigned) Node.Cases.size(), "rettmp");
        Builder.CreateRet(CasePhiNode);
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
    for (CurCase = Node.Cases.cbegin(), CaseBlock = CaseBlocks.cbegin(), CurPatVecBlock = PatVecBlocks.cbegin();
         CurCase != Node.Cases.cend();
         ++CurCase, ++CaseBlock, ++CurPatVecBlock) {
        (*CurCase)->accept(*this);
    }

    // Make entry point to first block
    Builder.SetInsertPoint(Entry);
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

    PatVals.clear();
    for (CurPat = Node.Patterns.cbegin(), CurArg = Args.begin(), PatBlock = CurPatVecBlock->cbegin();
         CurPat != Node.Patterns.cend();
         ++CurPat, ++CurArg, ++PatBlock) {

        // Set true block
        if (next(PatBlock) != CurPatVecBlock->cend())
            TrueBlock = *next(PatBlock);
        else
            TrueBlock = *CaseBlock;

        // Set false block
        if (next(CaseBlock) != CaseBlocks.cend())
            FalseBlock = (*next(CurPatVecBlock)).front();
        else
            FalseBlock = ErrBlock;

        PatRetBlock = BasicBlock::Create(Ctx, (*PatBlock)->getName() + "_ret", CurFunc);
        Builder.SetInsertPoint(*PatBlock);
        CurVal = *CurArg;
        (*CurPat)->accept(*this);
        Builder.CreateBr(PatRetBlock);
        Builder.SetInsertPoint(PatRetBlock);

        // Visit when expression
        if (Node.When)
            Node.When->accept(*this);

        // Create condition
        Builder.CreateCondBr(CurVal, TrueBlock, FalseBlock);
    }

    // Generate expression in case block
    Builder.SetInsertPoint(*CaseBlock);
    Node.Expr->accept(*this);

#ifdef SPPLDEBUG
    auto Ty1 = CurVal->getType();
    auto Ty2 = CasePhiNode->getType();
    assert(Ty1 == Ty2);
#endif

    // Ignore expression type if function is void
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID)
        CasePhiNode->addIncoming(CurVal, *CaseBlock);

    // Add return value to phi node
    Builder.CreateBr(CaseRetBlock);
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
    Entry = BasicBlock::Create(Ctx, "entry", Func);
    auto CheckBlock = BasicBlock::Create(Ctx, "check", Func);
    auto UpdateBlock = BasicBlock::Create(Ctx, "update", Func);
    auto LoopBlock = BasicBlock::Create(Ctx, "loop", Func);
    auto EndBlock = BasicBlock::Create(Ctx, "end", Func);

    // Init in entry block
    Builder.SetInsertPoint(Entry);
    auto Iter = Builder.CreateAlloca(Int32);
    Builder.CreateStore(ConstantInt::get(Int32, APInt(32, 0)), Iter);

    // Data
    CurVal = ConstantInt::get(Int32, APInt(32, DataLayout->getPointerSize()));
    CurVal = Builder.CreateMul(Argc, CurVal);
    auto Data = CreateMalloc(CurVal, Entry);

    // List
    //auto List = CreateList(Type, Data, Argc, Entry);
    assert(0);
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

    //Args.push_back(List);
    assert(0);

    return Func;
}

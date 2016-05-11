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

    if (verifyFunction(*CurFunc, &RawOut)) {
        addError(Error("LLVM Error"));
        RawOut.flush();
    }
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

    // Main must return Int32
    if (CurFunc->getName() == "main") {
        CurVal = Builder.CreateTrunc(CurVal, Int32, "tmptrunc");
    }

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
    auto CharListType = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});
    auto StrListType = common::Type(TypeId::LIST, vector<common::Type> { CharListType });

    // Create Function
    auto Func = llvm::Function::Create(MainType,
                                       llvm::Function::ExternalLinkage, "main",
                                       Module.get());

    // System arguments
    auto ArgIter = Func->args().begin();
    Argument *Argc = &*(ArgIter++);
    Argument *Argv = &*(ArgIter);
    Argc->setName("argc");
    Argv->setName("argv");

    // Create blocks
    Entry = BasicBlock::Create(Ctx, "entry", Func);
    auto CheckBlock = BasicBlock::Create(Ctx, "check", Func);
    auto StrAddBlock = BasicBlock::Create(Ctx, "update", Func);
    auto StrLoopBlock = BasicBlock::Create(Ctx, "str_loop", Func);
    auto CharLoopBlock = BasicBlock::Create(Ctx, "char_loop", Func);
    auto CharAddBlock = BasicBlock::Create(Ctx, "str_add", Func);
    auto EndBlock = BasicBlock::Create(Ctx, "end", Func);


    // Init in entry block
    Builder.SetInsertPoint(Entry);
    Value *StrList = Builder.CreateAlloca(getType(StrListType), nullptr, "str_list");
    Value *CharList = Builder.CreateAlloca(getType(CharListType), nullptr, "char_list");
    Value *StrIter = Builder.CreateAlloca(Int32, nullptr, "str_iter");
    Value *CharIter = Builder.CreateAlloca(Int32, nullptr, "char_iter");
    Value *Char = Builder.CreateAlloca(Int8, nullptr, "char");
    Value *CharPtr = Builder.CreateAlloca(PointerType::getUnqual(Int8), nullptr, "char_ptr");
    Value *TmpVal;
    Builder.CreateStore(ConstantInt::get(Int32, APInt(32, 0)), StrIter);
    Builder.CreateBr(CheckBlock);

    // Check block
    Builder.SetInsertPoint(CheckBlock);
    CurVal = Builder.CreateLoad(Int32, StrIter, "loadtmp");
    CurVal = Builder.CreateICmpULT(CurVal, Argc, "cmptmp");
    Builder.CreateCondBr(CurVal, StrLoopBlock, EndBlock);

    // String add block
    Builder.SetInsertPoint(StrAddBlock);
    CurVal = Builder.CreateLoad(CharList);
    TmpVal = Builder.CreateLoad(StrList);
    CurVal = CreateListNode(StrListType, CurVal, TmpVal, StrAddBlock, true);
    Builder.CreateStore(CurVal, StrList);

    CurVal = Builder.CreateLoad(Int32, StrIter, "loadtmp");
    CurVal = Builder.CreateAdd(CurVal, ConstantInt::get(Int32, APInt(32, 1)), "addtmp");
    Builder.CreateStore(CurVal, StrIter);
    Builder.CreateBr(CheckBlock);

    // String loop block    CurVal = Builder.CreateLoad(CharIter);
    Builder.SetInsertPoint(StrLoopBlock);
    CurVal = Builder.CreateLoad(StrIter);
    CurVal = Builder.CreateGEP(Argv, CurVal);
    CurVal = Builder.CreateLoad(CurVal);
    Builder.CreateStore(CurVal, CharPtr);
    Builder.CreateBr(CharLoopBlock);

    // Char loop block
    Builder.SetInsertPoint(CharLoopBlock);
    TmpVal = Builder.CreateLoad(CharPtr);
    CurVal = Builder.CreateLoad(CharIter);
    CurVal = Builder.CreateGEP(TmpVal, CurVal);
    CurVal = Builder.CreateLoad(Int8, CurVal);
    Builder.CreateStore(CurVal, Char);
    CurVal = Builder.CreateICmpEQ(CurVal, ConstantInt::get(Int8, 0));
    Builder.CreateCondBr(CurVal, StrAddBlock, CharAddBlock);

    // Char add block
    Builder.SetInsertPoint(CharAddBlock);
    CurVal = Builder.CreateLoad(Char);
    TmpVal = Builder.CreateLoad(CharList);
    CurVal = CreateListNode(CharListType, CurVal, TmpVal, CharAddBlock, true);
    Builder.CreateStore(CurVal, CharList);

    CurVal = Builder.CreateLoad(Int32, CharIter);
    CurVal = Builder.CreateAdd(CurVal, ConstantInt::get(Int32, APInt(32, 1)), "addtmp");
    Builder.CreateStore(CurVal, CharIter);
    Builder.CreateBr(CharLoopBlock);

    // EndBlock should be entry for the rest of function
    Entry = EndBlock;

    Args.push_back(StrList);

    return Func;
}

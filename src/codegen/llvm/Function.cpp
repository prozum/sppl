#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;

void LLVMCodeGen::visit(common::Function &Node) {

    // Create function and arguments
    Args.clear();
    if (Node.Id == string("main") && !Drv.JIT) {
        CurFunc = llvm::Function::Create(MainType,
                                         llvm::Function::ExternalLinkage, Node.Id,
                                         Module.get());
        Entry = BasicBlock::Create(Ctx, "entry", CurFunc);
        Args.push_back(CreateMainArg());
    }
    else {
        CurFunc = llvm::Function::Create(getFuncType(Node.Signature),
                                         llvm::Function::ExternalLinkage, Node.Id,
                                         Module.get());
        Entry = BasicBlock::Create(Ctx, "entry", CurFunc);
        // Setup names for arguments
        auto ArgId = 0;
        for (auto &Arg : CurFunc->args()) {
            Arg.setName("_arg" + to_string(ArgId++));
            Args.push_back(&Arg);
        }

    }

    // Set call convention to fast to enable tail recursion optimizations
    CurFunc->setCallingConv(CallingConv::Fast);

    // Create error block
    ErrBlock = BasicBlock::Create(Ctx, "error", CurFunc);
    Builder.SetInsertPoint(ErrBlock);
    if (!CurFunc->getReturnType()->isVoidTy())
        Builder.CreateRet(UndefValue::get(CurFunc->getReturnType()));
    else
        Builder.CreateRetVoid();

    // Create return block and setup phi node
    RetBlock = BasicBlock::Create(Ctx, "ret", CurFunc);
    Builder.SetInsertPoint(RetBlock);

    // Create ret instruction
    if (!CurFunc->getReturnType()->isVoidTy()) {
        CasePhiNode = Builder.CreatePHI(CurFunc->getReturnType(),
                                       (unsigned) Node.Cases.size(), "rettmp");
        Builder.CreateRet(CasePhiNode);
    } else {
        Builder.CreateRetVoid();
    }

    // Visit cases
    addPrefix("case");
    if (!Node.Cases.front()->Patterns.empty())
        CurPatBlock = BasicBlock::Create(Ctx, "", CurFunc);
    FirstBlock = true;
    for (CurCase = Node.Cases.cbegin(); CurCase != Node.Cases.cend(); ++CurCase) {
        if (next(CurCase) != Node.Cases.cend()) {
            if (!(*CurCase)->Patterns.empty())
                NextBlock = BasicBlock::Create(Ctx, "", CurFunc);
        } else {
            NextBlock = ErrBlock;
        }

        (*CurCase)->accept(*this);
        stepPrefix();
        CurPatBlock = NextBlock;
    }
    delPrefix();

    // Verify function and get error
    if (verifyFunction(*CurFunc, &RawOut)) {
        addError(Error("LLVM Error:\n" + ModuleString()));
        if (!Drv.Silent)
            RawOut.flush();
    }
}

void LLVMCodeGen::visit(common::Case &Node) {
    // Create case block
    CurCaseBlock = BasicBlock::Create(Ctx, getPrefix(), CurFunc);

    // Store tail recursion state in visitor
    TailRec = Node.TailRec;

    // Clear pattern values
    PatVals.clear();


    addPrefix("pat");
    if (!Node.Patterns.empty()) {
        CurPatBlock->setName(getPrefix());
    }

    // Set first block after entry
    Builder.SetInsertPoint(Entry);
    if (FirstBlock) {
        if (!Node.Patterns.empty()) {
            Builder.CreateBr(CurPatBlock);
        }
        FirstBlock = false;
    }

    // Visit patterns
    for (CurPat = Node.Patterns.cbegin(), CurArg = Args.cbegin();
         CurPat != Node.Patterns.cend();
         ++CurPat, ++CurArg) {

        // Visit pattern with current argument
        Builder.SetInsertPoint(CurPatBlock);
        CurVal = *CurArg;
        (*CurPat)->accept(*this);
        stepPrefix();

        if (next(CurPat) != Node.Patterns.cend()) {
            CurPatBlock = BasicBlock::Create(Ctx, getPrefix(), CurFunc);
            Builder.CreateCondBr(CurVal, CurPatBlock, NextBlock);
        }
    }
    delPrefix();

    // Create when block
    if (Node.When) {
        auto WhenBlock = BasicBlock::Create(Ctx, getPrefix() + "_when", CurFunc);
        Builder.CreateCondBr(CurVal, WhenBlock, NextBlock);
        Builder.SetInsertPoint(WhenBlock);
        Node.When->accept(*this);
        Builder.CreateCondBr(CurVal, CurCaseBlock, NextBlock);
    } else {
        if (!Node.Patterns.empty())
            Builder.CreateCondBr(CurVal, CurCaseBlock, NextBlock);
        else
            Builder.CreateBr(CurCaseBlock);
    }

    // Generate expression in case block
    Builder.SetInsertPoint(CurCaseBlock);
    Node.Expr->accept(*this);

    // Main must return Int32
    if (CurFunc->getName() == "main" && !Drv.JIT) {
        //CurVal = Builder.CreateTrunc(CurVal, Int32, "tmptrunc");
        CurVal = ConstantInt::get(Int32, 0);
    }

#ifdef SPPLDEBUG
    auto Ty1 = CurVal->getType();
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID) {
        auto Ty2 = CasePhiNode->getType();
        assert(Ty1 == Ty2);
    }
#endif

    // Ignore expression type if function is void
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID)
        CasePhiNode->addIncoming(CurVal, CurCaseBlock);

    // Add return value to phi node
    Builder.CreateBr(RetBlock);
}

llvm::Value *LLVMCodeGen::CreateMainArg()
{
    // Input Type
    auto CharListType = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});
    auto StrListType = common::Type(TypeId::LIST, vector<common::Type> { CharListType });

    // TODO
    return ConstantPointerNull::get(static_cast<PointerType *>(getType(StrListType)));

    // Create Function
    vector<llvm::Type *> types = {};
    auto CreateArg = llvm::Function::Create(FunctionType::get(getType(StrListType), vector<llvm::Type *> { Int32, PointerType::getUnqual(PointerType::getUnqual(Int8)) }, false),
                                       llvm::Function::ExternalLinkage, "create_arg",
                                       Module.get());

    // System arguments
    auto ArgIter = CreateArg->args().begin();
    Argument *Argc = &*(ArgIter++);
    Argument *Argv = &*(ArgIter);
    Argc->setName("argc");
    Argv->setName("argv");

    // Create blocks
    auto StartBlock = BasicBlock::Create(Ctx, "entry", CreateArg);
    auto CheckBlock = BasicBlock::Create(Ctx, "check", CreateArg);
    auto StrLoopBlock = BasicBlock::Create(Ctx, "str_loop", CreateArg);
    auto CharLoopBlock = BasicBlock::Create(Ctx, "char_loop", CreateArg);
    auto CharAddBlock = BasicBlock::Create(Ctx, "char_add", CreateArg);
    auto StrAddBlock = BasicBlock::Create(Ctx, "str_add", CreateArg);
    auto EndBlock = BasicBlock::Create(Ctx, "end", CreateArg);

    // Init in entry block
    Builder.SetInsertPoint(StartBlock);
    Value *AllocaStringList = Builder.CreateAlloca(getType(StrListType), nullptr, "str_list");
    Value *AllocaString = Builder.CreateAlloca(getType(CharListType), nullptr, "char_list");
    Value *AllocaStringIter = Builder.CreateAlloca(Int32, nullptr, "str_iter");
    Value *AllocaCharIter = Builder.CreateAlloca(Int32, nullptr, "char_iter");
    //Value *AllocaChar = Builder.CreateAlloca(Int8, nullptr, "char");
    //Value *CharPtr = Builder.CreateAlloca(VoidPtr, nullptr, "char_ptr");
    //Value *TmpVal;
    Builder.CreateStore(ConstantInt::get(Int32, 0), AllocaStringIter);
    Builder.CreateBr(CheckBlock);

    // Check block
    Builder.SetInsertPoint(CheckBlock);
    auto StringIter = Builder.CreateLoad(AllocaStringIter, "loadtmp");
    CurVal = Builder.CreateICmpULT(StringIter, Argc, "cmptmp");
    Builder.CreateCondBr(CurVal, StrLoopBlock, EndBlock);

    // String loop block
    Builder.SetInsertPoint(StrLoopBlock);
    Builder.CreateStore(ConstantInt::get(Int32, 0), AllocaCharIter);
    //CurVal = Builder.CreateLoad(AllocaStringIter);
    CurVal = Builder.CreateGEP(Argv, StringIter);
    auto CharPtr = Builder.CreateLoad(CurVal);
    Builder.CreateBr(CharLoopBlock);

    // Char loop block
    Builder.SetInsertPoint(CharLoopBlock);
    auto CharIter = Builder.CreateLoad(AllocaCharIter);
    CurVal = Builder.CreateGEP(CharPtr, CharIter); // ??
    auto Char = Builder.CreateLoad(Int8, CurVal);
    //Builder.CreateStore(CurVal, AllocaChar);
    CurVal = Builder.CreateICmpEQ(Char, ConstantInt::get(Int8, 0));
    Builder.CreateCondBr(CurVal, StrAddBlock, CharAddBlock);

    // Char add block
    Builder.SetInsertPoint(CharAddBlock);
    //CurVal = Builder.CreateLoad(AllocaChar);
    auto CharList = Builder.CreateLoad(AllocaString);
    //TmpVal = Builder.CreatePointerCast(CharList, getType(CharListType));
    CurVal = CreateListNode(CharListType, Char, CharList, CharAddBlock);
    Builder.CreateStore(CurVal, AllocaString);

    CharIter = Builder.CreateLoad(Int32, AllocaCharIter);
    CurVal = Builder.CreateAdd(CharIter, ConstantInt::get(Int32, APInt(32, 1)), "addtmp");
    Builder.CreateStore(CurVal, AllocaCharIter);
    Builder.CreateBr(CharLoopBlock);

    // String add block
    Builder.SetInsertPoint(StrAddBlock);
    auto StringList = Builder.CreateLoad(AllocaStringList, "loadtmp");
    auto String = Builder.CreateLoad(AllocaString);
    CurVal = CreateListNode(StrListType, String, StringList, StrAddBlock);
    Builder.CreateStore(CurVal, AllocaStringList);

    CurVal = Builder.CreateLoad(Int32, AllocaStringIter, "loadtmp");
    CurVal = Builder.CreateAdd(CurVal, ConstantInt::get(Int32, APInt(32, 1)), "addtmp");
    Builder.CreateStore(CurVal, AllocaStringIter);
    Builder.CreateBr(CheckBlock);

    // End block
    Builder.SetInsertPoint(EndBlock);
    CurVal = Builder.CreateLoad(AllocaStringList);
    Builder.CreateRet(CurVal);

    // Call create_arg
    Builder.SetInsertPoint(Entry);
    auto MainArgIter = CurFunc->args().begin();
    Argument *MainArgc = &*(MainArgIter++);
    Argument *MainArgv = &*(MainArgIter);
    MainArgc->setName("argc");
    MainArgv->setName("argv");

    if (verifyFunction(*CreateArg, &RawOut)) {
        addError(Error("LLVM Error:\n" + ModuleString()));
        if (!Drv.Silent)
            RawOut.flush();
    }

    return Builder.CreateCall(CreateArg, { MainArgc, MainArgv }, "_arg0");
}

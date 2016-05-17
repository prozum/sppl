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

        // System args
        auto ArgIter = CurFunc->args().begin();
        Argument *Argc = &*(ArgIter++);
        Argument *Argv = &*(ArgIter);
        Argc->setName("argc");
        Argv->setName("argv");

        // Call create arg function
        // TODO
        //CurVal = Builder.CreateCall(ArgFunc, vector<Value *> { Argc, Argv } );
        auto CharListType = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});
        auto StrListType = common::Type(TypeId::LIST, vector<common::Type> { CharListType });
        CurVal = ConstantPointerNull::get(static_cast<PointerType *>(getType(StrListType)));
        Args.push_back(CurVal);
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
    if (!CurFunc->getReturnType()->isVoidTy()) {
        RetPhiNode = Builder.CreatePHI(CurFunc->getReturnType(),
                                       (unsigned) Node.Cases.size(), "rettmp");
        Builder.CreateRet(RetPhiNode);
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

    // Set pattern prefix
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

        if (next(CurPat) != Node.Patterns.cend()) {
            stepPrefix();
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
    }

    // Branch to next block
    if (!Node.Patterns.empty())
        Builder.CreateCondBr(CurVal, CurCaseBlock, NextBlock);
    else
        Builder.CreateBr(CurCaseBlock);

    // Generate expression in case block
    Builder.SetInsertPoint(CurCaseBlock);
    Node.Expr->accept(*this);

    // Main must return Int32
    if (CurFunc->getName() == "main" && !Drv.JIT) {
        if (CurVal->getType()->isIntegerTy())
            CurVal = Builder.CreateTrunc(CurVal, Int32, "tmptrunc");
        else if (CurVal->getType()->isFloatTy())
            CurVal = Builder.CreateFPToSI(CurVal, Int32, "casttmp");
        else
            CurVal = ConstantInt::get(Int32, 0);
    }

#ifdef SPPLDEBUG
    auto Ty1 = CurVal->getType();
    if (CurFunc->getReturnType()->getTypeID() != llvm::Type::TypeID::VoidTyID) {
        auto Ty2 = RetPhiNode->getType();
        assert(Ty1 == Ty2);
    }
#endif

    // Add return value to phi node
    RetPhiNode->addIncoming(CurVal, CurCaseBlock);
    Builder.CreateBr(RetBlock);
}



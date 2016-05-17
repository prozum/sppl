#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;

void LLVMCodeGen::initStdLib() {
    // C function used by standard library
    addInternFunc("printf", FunctionType::get(Int32, VoidPtr, true));

    // Internal library
    createArgFunc();
    createPrintFunc();

    // External library
    auto DDSig = common::Type(TypeId::SIGNATURE, vector<common::Type> { common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT)});
    auto DDDSig = common::Type(TypeId::SIGNATURE, vector<common::Type> { common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT)});
    Drv.addDecl("sin", DDSig);
    //Drv.addDecl("asin", DDSig);
    Drv.addDecl("sinh", DDSig);
    Drv.addDecl("cos", DDSig);
    //Drv.addDecl("acos", DDSig);
    Drv.addDecl("cosh", DDSig);
    Drv.addDecl("tan", DDSig);
    Drv.addDecl("atan", DDSig);
    Drv.addDecl("tanh", DDSig);
    Drv.addDecl("pow", DDDSig);
    Drv.addDecl("exp", DDSig);
    Drv.addDecl("exp2", DDSig);
    Drv.addDecl("log", DDSig);
    Drv.addDecl("log10", DDSig);
    Drv.addDecl("fabs", DDSig);
    Drv.addDecl("floor", DDSig);
    Drv.addDecl("ceil", DDSig);
    Drv.addDecl("sqrt", DDSig);

    // Standard library
    // TODO
}

void LLVMCodeGen::addInternFunc(string FuncName, FunctionType *Ty) {
    InternFuncs[FuncName] = Ty;
}

llvm::Function *LLVMCodeGen::getInternFunc(string FuncName) {
    auto Decl = InternFuncDecls.find(FuncName);

    if (Decl != InternFuncDecls.end())
        return Decl->second;

    auto FuncType = InternFuncs[FuncName];

    return InternFuncDecls[FuncName] = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, FuncName, ModuleHeader.get());
}

void LLVMCodeGen::createArgFunc()
{
    // Input Type
    auto CharListType = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});
    auto StrListType = common::Type(TypeId::LIST, vector<common::Type> { CharListType });


    // Create Function
    auto FuncName = "create_arg";
    auto FuncTy = FunctionType::get(getType(StrListType), vector<llvm::Type *> { Int32, PointerType::getUnqual(VoidPtr) }, false);
    auto Func = llvm::Function::Create(FuncTy,
                                       llvm::Function::ExternalLinkage, "create_arg",
                                       ModuleHeader.get());

    // Arguments
    auto ArgIter = Func->args().begin();
    Argument *Argc = ArgIter++;
    Argument *Argv = ArgIter;
    Argc->setName("argc");
    Argv->setName("argv");

    // TODO
    auto Entry = BasicBlock::Create(Ctx, "entry", Func);
    Builder.SetInsertPoint(Entry);
    Builder.CreateRet(ConstantPointerNull::get(static_cast<PointerType *>(getType(StrListType))));

    addInternFunc(FuncName, FuncTy);
    //ArgFunc = llvm::Function::Create(CreateArg->getFunctionType(), llvm::Function::LinkageTypes::ExternalLinkage, "_create_arg", Module.get());

    /*
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
     */
}

void LLVMCodeGen::createPrintTupleFunc() {
    auto FuncName = "_print_tuple";
    auto FuncTy = FunctionType::get(llvm::Type::getVoidTy(Ctx), vector<llvm::Type *> { Int, PointerType::getUnqual(RuntimeType) }, false);
    auto Func = llvm::Function::Create(FuncTy,
                                        llvm::Function::ExternalLinkage, FuncName,
                                        ModuleHeader.get());

    // Arguments arguments
    auto ArgIter = Func->args().begin();
    Argument *DataArg = ArgIter++;
    Argument *TypeArg = ArgIter;
    DataArg->setName("data_arg");
    TypeArg->setName("type_arg");


    // Standard blocks
    auto Entry = BasicBlock::Create(Ctx, "entry", Func);
    auto Loop = BasicBlock::Create(Ctx, "loop", Func);
    auto Ret = BasicBlock::Create(Ctx, "ret", Func);
    auto Error = BasicBlock::Create(Ctx, "error", Func);

    // Setup return
    Builder.SetInsertPoint(Ret);
    // Print tuple end
    auto ParEnd = Builder.CreateGlobalStringPtr(")", "par_end");
    Builder.CreateCall(getInternFunc("printf"), vector<Value *> { ParEnd });
    Builder.CreateRetVoid();

    // Setup error
    Builder.SetInsertPoint(Error);
    Builder.CreateRetVoid();

    // Setup entry
    Builder.SetInsertPoint(Entry);
    auto DataAlloca = Builder.CreateAlloca(Int);
    auto TypeAlloca = Builder.CreateAlloca(PointerType::getUnqual(RuntimeType));
    // Initialize iterator
    auto IterAlloca = Builder.CreateAlloca(Int);
    Builder.CreateStore(ConstantInt::get(Int, 0), IterAlloca);
    // Print tuple start
    auto ParStart = Builder.CreateGlobalStringPtr("(", "par_start");
    Builder.CreateCall(getInternFunc("printf"), vector<Value *> { ParStart });
    Builder.CreateBr(Loop);

    // Setup loop
    Builder.SetInsertPoint(Loop);
    auto Iter = Builder.CreateLoad(IterAlloca);
    auto Data = Builder.CreateGEP(RuntimeType, DataArg, Iter);
    auto Type = Builder.CreateGEP(RuntimeType, TypeArg, Iter);

    Builder.CreateCall(InternFuncDecls["_print"], vector<Value *> { Data, Type });

    // Increment iterator
    CurVal = Builder.CreateAdd(Iter, ConstantInt::get(Int, 1));
    Builder.CreateStore(CurVal, IterAlloca);

    //Builder.CreateStore(DataArg, UnionArgCast);
    //auto UnionArgCast = Builder.CreateBitCast()

    InternFuncs[FuncName] = FuncTy;
    InternFuncDecls[FuncName] = Func;
}

void LLVMCodeGen::createPrintFunc() {
    auto FuncName = "_print";
    auto FuncTy = FunctionType::get(llvm::Type::getVoidTy(Ctx), vector<llvm::Type *> { Int, PointerType::getUnqual(RuntimeType) }, false);
    auto Func = llvm::Function::Create(FuncTy,
                                        llvm::Function::ExternalLinkage, FuncName,
                                        ModuleHeader.get());

    // Arguments arguments
    auto ArgIter = Func->args().begin();
    Argument *DataArg = ArgIter++;
    Argument *TypeArg = ArgIter;
    DataArg->setName("data_arg");
    TypeArg->setName("type_arg");

    // Printf
    auto PrintF = getInternFunc("printf");

    // Standard blocks
    auto Entry = BasicBlock::Create(Ctx, "entry", Func);
    auto Ret = BasicBlock::Create(Ctx, "ret", Func);
    auto Error = BasicBlock::Create(Ctx, "error", Func);

    // Switch blocks
    auto PrintInt = BasicBlock::Create(Ctx, "print_int", Func);
    auto PrintFloat = BasicBlock::Create(Ctx, "print_float", Func);
    auto PrintChar = BasicBlock::Create(Ctx, "print_char", Func);

    auto PrintBool = BasicBlock::Create(Ctx, "print_bool", Func);
    auto PrintTrue = BasicBlock::Create(Ctx, "print_true", Func);
    auto PrintFalse = BasicBlock::Create(Ctx, "print_false", Func); /*

    auto PrintString = BasicBlock::Create(Ctx, "print_string", Print);
    auto PrintList = BasicBlock::Create(Ctx, "print_list", Print);
    auto PrintTuple = BasicBlock::Create(Ctx, "print_tuple", Print);
    auto PrintSignature = BasicBlock::Create(Ctx, "print_signature", Print);
    auto PrintGeneric = BasicBlock::Create(Ctx, "print_generic", Print);
    auto PrintEmptyList = BasicBlock::Create(Ctx, "print_empty_list", Print);
    auto PrintCustom = BasicBlock::Create(Ctx, "print_custom", Print);
    auto PrintVoid = BasicBlock::Create(Ctx, "print_void", Print); */

    // Setup return
    Builder.SetInsertPoint(Ret);
    Builder.CreateRetVoid();

    // Setup error
    Builder.SetInsertPoint(Error);
    Builder.CreateRetVoid();

    // Setup entry
    Builder.SetInsertPoint(Entry);
    auto UnionArgAlloca = Builder.CreateAlloca(UnionType);
    auto UnionArgGEP = Builder.CreateStructGEP(UnionType, UnionArgAlloca, 0);
    auto UnionArgCast = Builder.CreateBitCast(UnionArgGEP, PointerType::getUnqual(Int));
    Builder.CreateStore(DataArg, UnionArgCast);
    //auto UnionArgCast = Builder.CreateBitCast()

    auto CurVal = Builder.CreateStructGEP(RuntimeType, TypeArg, 0);
    CurVal = Builder.CreateLoad(Int, CurVal);
    auto Switch = Builder.CreateSwitch(CurVal, Error, (unsigned int)common::TypeId::TYPES);
    Switch->addCase(ConstantInt::get(Int, (uint64_t)common::TypeId::INT), PrintInt);
    Switch->addCase(ConstantInt::get(Int, (uint64_t)common::TypeId::FLOAT), PrintFloat);
    Switch->addCase(ConstantInt::get(Int, (uint64_t)common::TypeId::CHAR), PrintChar);
    Switch->addCase(ConstantInt::get(Int, (uint64_t)common::TypeId::BOOL), PrintBool); /*
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::STRING), PrintString);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::LIST), PrintList);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::TUPLE), PrintTuple);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::SIGNATURE), PrintSignature);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::GENERIC), PrintGeneric);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::EMPTYLIST), PrintEmptyList);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::CUSTOM), PrintCustom);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::VOID), PrintVoid);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::FLOAT), PrintChar);*/

    // Setup print int
    Builder.SetInsertPoint(PrintInt);
    auto IntFormat = Builder.CreateGlobalStringPtr("%i", "print_format_int");
    auto IntCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Int));
    auto IntLoad  = Builder.CreateLoad(IntCast);
    Builder.CreateCall(PrintF, vector<Value *> { IntFormat, IntLoad });
    Builder.CreateBr(Ret);

    // Setup print float
    Builder.SetInsertPoint(PrintFloat);
    auto FloatFormat = Builder.CreateGlobalStringPtr("%f", "print_format_float");
    auto FloatCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Float));
    auto FloatLoad  = Builder.CreateLoad(FloatCast);
    Builder.CreateCall(PrintF, vector<Value *> { FloatFormat, FloatLoad });
    Builder.CreateBr(Ret);

    // Setup print char
    Builder.SetInsertPoint(PrintChar);
    auto CharFormat = Builder.CreateGlobalStringPtr("%c", "print_format_char");
    auto CharCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Int));
    auto CharLoad  = Builder.CreateLoad(CharCast);
    Builder.CreateCall(PrintF, vector<Value *> { CharFormat, CharLoad });
    Builder.CreateBr(Ret);

    // Setup print bool
    Builder.SetInsertPoint(PrintBool);
    auto BoolCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Int1));
    auto BoolLoad  = Builder.CreateLoad(BoolCast);
    Builder.CreateCondBr(BoolLoad, PrintTrue, PrintFalse);

    // Setup print true
    Builder.SetInsertPoint(PrintTrue);
    auto BoolTrue = Builder.CreateGlobalStringPtr("True", "print_true");
    Builder.CreateCall(PrintF, vector<Value *> { BoolTrue });
    Builder.CreateBr(Ret);

    // Setup print false
    Builder.SetInsertPoint(PrintFalse);
    auto BoolFalse = Builder.CreateGlobalStringPtr("False", "print_true");
    Builder.CreateCall(PrintF, vector<Value *> { BoolFalse });
    Builder.CreateBr(Ret);

    addInternFunc(FuncName, FuncTy);
}

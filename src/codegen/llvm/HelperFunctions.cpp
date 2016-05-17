#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;

llvm::Function *LLVMCodeGen::CreateArgFunc()
{
    // Input Type
    auto CharListType = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});
    auto StrListType = common::Type(TypeId::LIST, vector<common::Type> { CharListType });


    // Create Function
    vector<llvm::Type *> Types = {};
    auto CreateArg = llvm::Function::Create(FunctionType::get(getType(StrListType), vector<llvm::Type *> { Int32, PointerType::getUnqual(PointerType::getUnqual(Int8)) }, false),
                                       llvm::Function::ExternalLinkage, "create_arg",
                                       Module.get());

    // System arguments
    auto ArgIter = CreateArg->args().begin();
    Argument *Argc = &*(ArgIter++);
    Argument *Argv = &*(ArgIter);
    Argc->setName("argc");
    Argv->setName("argv");

    // TODO
    auto Entry = BasicBlock::Create(Ctx, "entry", CreateArg);
    Builder.SetInsertPoint(Entry);
    Builder.CreateRet(ConstantPointerNull::get(static_cast<PointerType *>(getType(StrListType))));
    return CreateArg;

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

llvm::Function *LLVMCodeGen::CreatePrintFunc() {
    auto PrintTy = FunctionType::get(llvm::Type::getVoidTy(Ctx), vector<llvm::Type *> { Int64, PointerType::getUnqual(RuntimeType) }, false);
    auto Print = llvm::Function::Create(PrintTy,
                                        llvm::Function::ExternalLinkage, "_print",
                                        Module.get());

    // Arguments arguments
    auto ArgIter = Print->args().begin();
    Argument *DataArg = &*(ArgIter++);
    Argument *TypeArg = &*(ArgIter);
    DataArg->setName("data_arg");
    TypeArg->setName("type_arg");

    // Printf
    auto PrintFType = FunctionType::get(Int32, VoidPtr, true);
    auto PrintF = llvm::Function::Create(PrintFType, llvm::Function::LinkageTypes::ExternalLinkage, "printf", Module.get());

    // Standard blocks
    auto Entry = BasicBlock::Create(Ctx, "entry", Print);
    auto Ret = BasicBlock::Create(Ctx, "ret", Print);
    auto Error = BasicBlock::Create(Ctx, "error", Print);

    // Switch blocks
    auto PrintInt = BasicBlock::Create(Ctx, "print_int", Print);
    auto PrintFloat = BasicBlock::Create(Ctx, "print_float", Print);
    auto PrintChar = BasicBlock::Create(Ctx, "print_char", Print);

    auto PrintBool = BasicBlock::Create(Ctx, "print_bool", Print);
    auto PrintTrue = BasicBlock::Create(Ctx, "print_true", Print);
    auto PrintFalse = BasicBlock::Create(Ctx, "print_false", Print); /*

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
    auto UnionArgCast = Builder.CreateBitCast(UnionArgGEP, PointerType::getUnqual(Int64));
    Builder.CreateStore(DataArg, UnionArgCast);
    //auto UnionArgCast = Builder.CreateBitCast()

    auto CurVal = Builder.CreateStructGEP(RuntimeType, TypeArg, 0);
    CurVal = Builder.CreateLoad(Int64, CurVal);
    auto Switch = Builder.CreateSwitch(CurVal, Error, (unsigned int)common::TypeId::TYPES);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::INT), PrintInt);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::FLOAT), PrintFloat);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::CHAR), PrintChar);
    Switch->addCase(ConstantInt::get(Int64, (uint64_t)common::TypeId::BOOL), PrintBool); /*
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
    auto IntCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Int64));
    auto IntLoad  = Builder.CreateLoad(IntCast);
    Builder.CreateCall(PrintF, vector<Value *> { IntFormat, IntLoad });
    Builder.CreateBr(Ret);

    // Setup print float
    Builder.SetInsertPoint(PrintFloat);
    auto FloatFormat = Builder.CreateGlobalStringPtr("%f", "print_format_float");
    auto FloatCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Double));
    auto FloatLoad  = Builder.CreateLoad(FloatCast);
    Builder.CreateCall(PrintF, vector<Value *> { FloatFormat, FloatLoad });
    Builder.CreateBr(Ret);

    // Setup print char
    Builder.SetInsertPoint(PrintChar);
    auto CharFormat = Builder.CreateGlobalStringPtr("%c", "print_format_char");
    auto CharCast  = Builder.CreateBitCast(UnionArgAlloca, PointerType::getUnqual(Int8));
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

    return Print;
}
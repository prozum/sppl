#include "LLVMCodeGen.h"
#include "std/std-bin.h"

using namespace std;
using namespace llvm;
using namespace codegen;
using namespace common;
static std::unique_ptr<llvm::Module> ModuleStd;

llvm::Module *LLVMCodeGen::getStdLib() {
    return ModuleStd.get();
}

std::unique_ptr<llvm::Module> LLVMCodeGen::getStdModule() {
    return move(ModuleStd);
}

void LLVMCodeGen::setTriple() {
    // Set triple from target machine
    Module->setTargetTriple(Machine->getTargetTriple().getTriple());
}

void LLVMCodeGen::initStdLib() {
    // Load standard library implemented in C
    if (!ModuleStd) {
        SMDiagnostic Dia;
        auto MemBuf = MemoryBuffer::getMemBuffer(StringRef((char *) std_bc, std_bc_len));
        ModuleStd = parseIR(MemBuf->getMemBufferRef(), Dia, Ctx);
        ModuleStd->setTargetTriple(Machine->getTargetTriple().getTriple());

        //ModuleStd->dump();
        assert(ModuleStd && "Standard Library could not be parsed");


        // Get annotations
        auto Annos = ModuleStd->getNamedGlobal("llvm.global.annotations");
        if (Annos) {
            auto Struct1 = static_cast<ConstantArray *>(Annos->getOperand(0));
            for (size_t i = 0; i < Struct1->getNumOperands(); ++i) {
                auto Struct2 = static_cast<ConstantStruct *>(Struct1->getOperand(i));
                if (auto Func = static_cast<llvm::Function *>(Struct2->getOperand(0)->getOperand(0))) {
                    auto Anno = cast<ConstantDataArray>(
                            cast<GlobalVariable>(Struct2->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();
                    Func->addFnAttr(Anno);
                }
            }
        }
    }

    // Load standard library function signatures
    for (auto &Func: ModuleStd->functions()) {
        addStdFunc(Func.getName(), Func.getFunctionType(), Func.hasFnAttribute("sppl_decl"));
    }

    // Load standard library types
    RunType = ModuleStd->getTypeByName("struct.type");
    UnionType = ModuleStd->getTypeByName("union.value");
    ListType = ModuleStd->getTypeByName("struct.list");

    // Load standard library implemented in IR
    //createArgFunc();
    //createPrintFunc();

    // SPPL declaration to access standard library
    auto DDSig = common::Type(TypeId::SIGNATURE, vector<common::Type> { common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT)});
    auto DDDSig = common::Type(TypeId::SIGNATURE, vector<common::Type> { common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT), common::Type(TypeId::FLOAT)});
    vector<string> Funcs = {
        "sin", "asin",  "sinh",
        "cos", "acos",  "cosh",
        "tan", "atan",  "tanh",
        "pow", "exp",   "exp2",
        "log", "log10", "fabs",
        "floor", "ceil", "sqrt"
    };
    addStdFunc("tan", DDSig);
    addStdFunc("pow", DDDSig);

    for (auto &Func : Funcs) {
        addStdFunc(Func, DDSig, true);
    }
}

void LLVMCodeGen::addStdFunc(std::string FuncName, common::Type Ty, bool Decl) {
    assert(Ty.Id == TypeId::SIGNATURE);

    StdFuncs[FuncName] = getLLVMFuncType(Ty);

    if (Decl)
        Drv.addDecl(FuncName, Ty);
}



void LLVMCodeGen::addStdFunc(string FuncName, FunctionType *Ty, bool Decl) {
    StdFuncs[FuncName] = Ty;

    if (Decl)
        Drv.addDecl(FuncName, getFuncType(Ty));
}

llvm::Function *LLVMCodeGen::getStdFunc(string FuncName) {
    auto Decl = StdFuncDecls.find(FuncName);

    if (Decl != StdFuncDecls.end())
        return Decl->second;

    auto FuncType = StdFuncs[FuncName];

    return StdFuncDecls[FuncName] = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, FuncName, Module.get());
}

void LLVMCodeGen::createArgFunc()
{
    // Input Type
    auto CharListType = common::Type(TypeId::STRING, vector<common::Type> { common::Type(TypeId::CHAR)});
    auto StrListType = common::Type(TypeId::LIST, vector<common::Type> { CharListType });


    // Create Function
    auto FuncName = "create_arg";
    auto FuncTy = FunctionType::get(getLLVMType(StrListType), vector<llvm::Type *> { Int32, PointerType::getUnqual(VoidPtr) }, false);
    auto Func = llvm::Function::Create(FuncTy,
                                       llvm::Function::ExternalLinkage, "create_arg",
                                       Module.get());

    // Arguments
    auto ArgIter = Func->args().begin();
    Argument *Argc = &*(ArgIter++);
    Argument *Argv = &*(ArgIter);
    Argc->setName("argc");
    Argv->setName("argv");

    // TODO
    auto Entry = BasicBlock::Create(Ctx, "entry", Func);
    Builder.SetInsertPoint(Entry);
    Builder.CreateRet(ConstantPointerNull::get(static_cast<PointerType *>(getLLVMType(StrListType))));

    addStdFunc(FuncName, FuncTy);
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

void LLVMCodeGen::createPrintFunc() {
    auto FuncName = "_print";
    auto FuncTy = FunctionType::get(llvm::Type::getVoidTy(Ctx), vector<llvm::Type *> { Int, PointerType::getUnqual(RunType) }, false);
    auto Func = llvm::Function::Create(FuncTy,
                                        llvm::Function::ExternalLinkage, FuncName,
                                        Module.get());

    // Arguments arguments
    auto ArgIter = Func->args().begin();
    Argument *DataArg = &*(ArgIter++);
    Argument *TypeArg = &*(ArgIter);
    DataArg->setName("data_arg");
    TypeArg->setName("type_arg");

    // Printf
    auto PrintF = getStdFunc("printf");

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

    auto CurVal = Builder.CreateStructGEP(RunType, TypeArg, 0);
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

    addStdFunc(FuncName, FuncTy);
}

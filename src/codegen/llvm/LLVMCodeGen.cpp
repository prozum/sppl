#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;

LLVMCodeGen::LLVMCodeGen(parser::Driver &Drv)
        : CodeGenerator(Drv),
          Ctx(getGlobalContext()),
          Builder(Ctx) {

    // Init LLVM before initializing TagetMachine and DataLayout
    initLLVM();
    Machine = std::unique_ptr<TargetMachine>(EngineBuilder().selectTarget());
    DataLayout = std::make_unique<llvm::DataLayout>(Machine->createDataLayout());
    Module = std::make_unique<llvm::Module>("SpplModule", Ctx);

    // Type constants
    Int1 = llvm::Type::getInt1Ty(Ctx);
    Int8 = llvm::Type::getInt8Ty(Ctx);
    Int32 = llvm::Type::getInt32Ty(Ctx);
    Int64 = llvm::Type::getInt64Ty(Ctx);
    Double = llvm::Type::getDoubleTy(Ctx);
    MainType = FunctionType::get(Int32,
                                 vector<Type *> {
                                         Int32,
                                         PointerType::getUnqual(PointerType::getUnqual(Int8))
                                 },
                                 false);
}

void LLVMCodeGen::visit(common::Program &node) {
    for (auto &func : node.Decls) {
        func->accept(*this);
    }

    // Don't output anything with JIT
    if (Drv.JIT)
        return;

    // Generate Binary or IR Source
    if (Drv.Binary) {
        raw_os_ostream RawOut(*Drv.Out);
        WriteBitcodeToFile(Module.get(), RawOut);
    }
    else {
        *Drv.Out << ModuleString();
    }
}

string LLVMCodeGen::ModuleString() {
    string ModuleStr;
    raw_string_ostream out(ModuleStr);
    out << *Module.get();
    return out.str();
}

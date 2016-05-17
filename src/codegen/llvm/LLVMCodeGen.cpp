#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;

LLVMCodeGen::LLVMCodeGen(parser::Driver &Drv)
        : CodeGenerator(Drv),
          Ctx(getGlobalContext()),
          Builder(Ctx),
          MsgOut(*Drv.MOut) {

    // Initialize LLVM before TargetMachine and DataLayout
    initLLVM();
    Machine = std::unique_ptr<TargetMachine>(EngineBuilder().selectTarget());
    DataLayout = std::make_unique<llvm::DataLayout>(Machine->createDataLayout());
    Module = std::make_unique<llvm::Module>("Module", Ctx);
    ModuleHeader = std::make_unique<llvm::Module>("ModuleHeader", Ctx);

    initTypes();
    initStdLib();
}

void LLVMCodeGen::visit(common::Program &node) {
    // Clear declaration and runtime types from last run
    InternFuncDecls.clear();
    RuntimeTypes.clear();

    for (auto &func : node.Decls) {
        func->accept(*this);
    }

    // Don't output anything with JIT
    if (Drv.JIT)
        return;

    // Generate Binary or IR Source
    raw_os_ostream RawOut(*Drv.Out);
    raw_os_ostream RawHOut(*Drv.HOut);
    if (Drv.Binary) {
        WriteBitcodeToFile(Module.get(), RawOut);
        WriteBitcodeToFile(ModuleHeader.get(), RawHOut);
    } else {
        RawOut << *Module;
        RawHOut << *ModuleHeader;
    }
}

string LLVMCodeGen::ModuleString() {
    string ModuleStr;
    raw_string_ostream Out(ModuleStr);
    Out << *Module.get();
    return Out.str();
}

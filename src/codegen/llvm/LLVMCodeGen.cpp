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

    // Add optimization passes
    PassMgr = std::make_unique<legacy::PassManager>();
    PassMgr->add(createTailCallEliminationPass());
    PassMgr->add(createInstructionCombiningPass());
    PassMgr->add(createReassociatePass());
    PassMgr->add(createGVNPass());
    PassMgr->add(createCFGSimplificationPass());

    // Initialize types and standard library
    initTypes();
    initStdLib();
}

void LLVMCodeGen::createModule() {
    Module = std::make_unique<llvm::Module>("Module", Ctx);
    setTriple();

    StdFuncDecls.clear();
    RunTypes.clear();
}

void LLVMCodeGen::visit(common::Program &node) {
    createModule();

    for (auto &func : node.Decls) {
        func->accept(*this);
    }

    if (!Drv.Silent) {
        *Drv.MOut << "#-----------------------------------------------------------------"
                "-------------#"
        << endl;
        *Drv.MOut << "|                                 Unoptimized                     "
                "             |"
        << endl;
        *Drv.MOut << "#-----------------------------------------------------------------"
                "-------------#"
        << endl;
        *Drv.MOut << ModuleString();
    }

    // Run optimizations
    PassMgr->run(*Module);

    if (!Drv.Silent) {
        *Drv.MOut << "#-----------------------------------------------------------------"
                "-------------#"
        << endl;
        *Drv.MOut << "|                                  Optimized                      "
                "             |"
        << endl;
        *Drv.MOut << "#-----------------------------------------------------------------"
                "-------------#"
        << endl;
        *Drv.MOut << ModuleString();
    }

    // Don't output anything with JIT
    if (Drv.JIT)
        return;

    // Generate Binary or IR Source
    raw_os_ostream RawOut(*Drv.Out);
    raw_os_ostream RawHOut(*Drv.HOut);
    if (Drv.Binary) {
        WriteBitcodeToFile(Module.get(), RawOut);
        WriteBitcodeToFile(getStdLib(), RawHOut);
    } else {
        RawOut << *Module;
        RawHOut << *getStdLib();
    }
}

string LLVMCodeGen::ModuleString() {
    string ModuleStr;
    raw_string_ostream Out(ModuleStr);
    Out << *Module.get();
    return Out.str();
}

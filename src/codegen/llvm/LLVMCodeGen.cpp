#include "LLVMCodeGen.h"

using namespace std;
using namespace llvm;
using namespace codegen;

LLVMCodeGen::LLVMCodeGen(parser::Driver &Drv)
        : CodeGenerator(Drv),
          Builder(Ctx),
          MsgOut(*Drv.MOut) {

    // Initialize LLVM before TargetMachine and DataLayout
    initLLVM();
    Machine = std::unique_ptr<TargetMachine>(EngineBuilder().selectTarget());
    DataLayout = std::make_unique<llvm::DataLayout>(Machine->createDataLayout());

    // Initialize types and standard library
    initTypes();
    initStdLib();
}

void LLVMCodeGen::setupOptimization() {
    PassMgr = std::make_unique<legacy::PassManager>();

    // Level 0
    PassMgr->add(createTailCallEliminationPass());

    // Level 1
    if (Drv.OptLevel >= 1) {
        PassMgr->add(createInstructionCombiningPass());
        PassMgr->add(createReassociatePass());
        PassMgr->add(createGVNPass());
        PassMgr->add(createCFGSimplificationPass());
    }
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
        *Drv.MOut << moduleString();
    }

    // Run optimizations
    setupOptimization();
    PassMgr->run(*Module);

    if (!Drv.Silent && Drv.OptLevel > 0) {
        *Drv.MOut << "#-----------------------------------------------------------------"
                "-------------#"
        << endl;
        *Drv.MOut << "|                                  Optimized                      "
                "             |"
        << endl;
        *Drv.MOut << "#-----------------------------------------------------------------"
                "-------------#"
        << endl;
        *Drv.MOut << moduleString();
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

string LLVMCodeGen::moduleString() {
    string ModuleStr;
    raw_string_ostream Out(ModuleStr);
    Out << *Module.get();
    return Out.str();
}

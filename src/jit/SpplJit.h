#pragma once

#include "Compiler.h"
#include "Driver.h"
#include "ScopeGenerator.h"
#include "TypeChecker.h"
#include "LLVMCodeGen.h"

#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>

#include <llvm/IR/Mangler.h>
#include <llvm/IR/Module.h>

#include <llvm/Support/DynamicLibrary.h>

#include <llvm/Analysis/Passes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>

namespace jit {
class SpplJit {
  public:
    SpplJit();

    int eval(std::string Str);

private:
    typedef llvm::orc::ObjectLinkingLayer<> ObjLayerT;
    typedef llvm::orc::IRCompileLayer<ObjLayerT> CompileLayerT;
    typedef CompileLayerT::ModuleSetHandleT ModuleHandleT;

    void createModule();
    ModuleHandleT addModule(std::unique_ptr<llvm::Module> M);
    void removeModule(ModuleHandleT Handler);

    llvm::orc::JITSymbol findSymbol(const std::string Name);
    llvm::orc::JITSymbol findMangledSymbol(const std::string &Name);
    std::string mangle(const std::string &Name);

    std::string getOutput(intptr_t Data, common::Type Type);
    std::string getOutputTuple(intptr_t Addr, common::Type Ty);
    std::string getOutputList(intptr_t Addr, common::Type);
    std::string getOutputString(intptr_t Addr, common::Type);

    parser::Driver Drv;
    codegen::LLVMCodeGen CodeGen;
    semantics::ScopeGenerator ScopeGen;
    semantics::TypeChecker TypeChecker;
#if OPTIMIZER
    optimizer::GeneralOptimizer Optimizer;
#endif

    ObjLayerT ObjectLayer;
    CompileLayerT CompileLayer;

    std::unique_ptr<llvm::legacy::FunctionPassManager> PassMgr;
    std::vector<ModuleHandleT> ModuleHandles;
    ModuleHandleT ModuleHandler;


    template <typename T> static std::vector<T> singletonSet(T Item) {
        std::vector<T> Vec;
        Vec.push_back(std::move(Item));
        return Vec;
    }

};
}

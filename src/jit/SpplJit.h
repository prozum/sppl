#pragma once

#include "Compiler.h"
#include "Driver.h"
#include "TypeChecker.h"
#include "ScopeGenerator.h"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/Mangler.h>

#include <llvm/Support/DynamicLibrary.h>

#include <llvm/Analysis/Passes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>

using namespace llvm;
using namespace llvm::orc;

namespace jit {
    class SpplJit {
    public:
        SpplJit();

        int eval(string Str);

        static void initLLVM() {
            InitializeNativeTarget();
            InitializeNativeTargetAsmPrinter();
            InitializeNativeTargetAsmParser();
        }

        typedef ObjectLinkingLayer<> ObjLayerT;
        typedef IRCompileLayer<ObjLayerT> CompileLayerT;
        typedef CompileLayerT::ModuleSetHandleT ModuleHandleT;

        void createModule();
        ModuleHandleT addModule(std::unique_ptr<llvm::Module> M);
        void removeModule(ModuleHandleT Handler);

        JITSymbol findSymbol(const std::string Name);
        JITSymbol findMangledSymbol(const std::string &Name);
        std::string mangle(const std::string &Name);

        string getOutput(intptr_t data, common::Type Type);
        string getOutputTuple(intptr_t addr, vector<common::Type> Subtypes);

        std::unique_ptr<TargetMachine> Machine;
        const DataLayout Layout;
        ObjLayerT ObjectLayer;
        CompileLayerT CompileLayer;

        std::unique_ptr<legacy::FunctionPassManager> PassMgr;
        std::vector<ModuleHandleT> ModuleHandles;
        ModuleHandleT ModuleHandler;

        parser::Driver Drv;
        codegen::LLVMCodeGenerator CodeGen;
        semantics::ScopeGenerator ScopeGen;
        semantics::TypeChecker TypeChecker;
        optimizer::GeneralOptimizer Optimizer;


        template<typename T>
        static std::vector<T> singletonSet(T Item) {
            std::vector<T> Vec;
            Vec.push_back(std::move(Item));
            return Vec;
        }
    };
}

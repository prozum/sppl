#pragma once
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/Mangler.h>

#include <llvm/Support/DynamicLibrary.h>

//#include <llvm/ADT/STLExtras.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>

#include "Compiler.h"
#include "Driver.h"
#include "TypeChecker.h"
#include "ScopeGenerator.h"

using namespace llvm;
using namespace llvm::orc;

namespace jit {
    class SpplJit {
    public:
        SpplJit(shared_ptr<ostream> out);

        void Eval(string str);

        static void Init_llvm() {
            InitializeNativeTarget();
            InitializeNativeTargetAsmPrinter();
            InitializeNativeTargetAsmParser();
        }

    private:
        typedef ObjectLinkingLayer<> ObjLayerT;
        typedef IRCompileLayer<ObjLayerT> CompileLayerT;
        typedef CompileLayerT::ModuleSetHandleT ModuleHandleT;

        void init_module_passmanager();

        ModuleHandleT add_module(std::unique_ptr<llvm::Module> m);

        void remove_module(ModuleHandleT handler);

        JITSymbol find_symbol(const std::string name);

        JITSymbol find_mangled_symbol(const std::string &name);

        std::string mangle(const std::string &name);

        std::unique_ptr<TargetMachine> Machine;
        const DataLayout Layout;
        ObjLayerT ObjectLayer;
        CompileLayerT CompileLayer;
        std::vector<ModuleHandleT> ModuleHandles;
        std::unique_ptr<legacy::FunctionPassManager> PassManager;

        semantics::ScopeGenerator ScopeGenerator;
        semantics::TypeChecker TypeChecker;
        codegen::LLVMCodeGenerator Generator;
        parser::Driver Driver;

        string get_output(intptr_t data, common::Type *node_type);

        string get_tuple_output(intptr_t addr, vector<shared_ptr<common::Type>> node_type);

        template<typename T>
        static std::vector<T> singletonSet(T t) {
            std::vector<T> Vec;
            Vec.push_back(std::move(t));
            return Vec;
        }
    };
}
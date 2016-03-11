#pragma once
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <iostream>

#include "CodeGenerator.h"


namespace codegen {

    class LLVMCodeGenerator : public common::CodeGenerator
    {
    public:
        LLVMCodeGenerator(std::ostream &);

        void visit(common::Program &node);
        void visit(common::Function &node);
        void visit(common::Case &node);
        void visit(common::Add &node);
        void visit(common::Float &node);

    private:
        llvm::IRBuilder<> Builder;
        llvm::Module Module;
        std::map<std::string, llvm::Value*> ContextValues;
        common::Function *current_func;
        llvm::Value *current_val;
    };
}

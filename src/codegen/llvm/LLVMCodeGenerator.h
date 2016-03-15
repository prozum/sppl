#pragma once
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <iostream>

#include "CodeGenerator.h"

namespace codegen {

class LLVMCodeGenerator : public common::CodeGenerator {
  public:
    LLVMCodeGenerator(std::ostream &);

    void visit(common::Program &node);
    void visit(common::Function &node);
    void visit(common::Case &node);
    void visit(common::Add &node);
    void visit(common::Float &node);
    void visit(common::Call &node);

    llvm::Function *create_function(common::Function *func);

    unique_ptr<llvm::Module> Module;

private:
    llvm::IRBuilder<> Builder;
    std::map<std::string, llvm::Value *> ContextValues;
    llvm::Function *cur_func;
    llvm::Value *cur_val;

    void visit(common::Id);

    void visit(common::Id &node);
};
}

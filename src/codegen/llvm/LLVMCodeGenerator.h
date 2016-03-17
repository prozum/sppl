#pragma once
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/ValueSymbolTable.h>

#include <iostream>

#include "CodeGenerator.h"

namespace codegen {

    enum Context {
        PATTERN,
        EXPR,
    };

class LLVMCodeGenerator : public common::CodeGenerator {
  public:
    LLVMCodeGenerator(std::ostream &);

    void visit(common::Program &node);
    void visit(common::Function &node);
    void visit(common::Case &node);
    void visit(common::Add &node);
    void visit(common::Float &node);
    void visit(common::Call &node);


    llvm::IRBuilder<> Builder;
    unique_ptr<llvm::Module> Module;
    llvm::Function *cur_func;

private:
    std::vector<llvm::Argument *> Arguments;
    std::map<std::string, llvm::Value *> ContextValues;
    llvm::Value *cur_val;
    int case_id;
    Context ctx;

    void visit(common::Id);

    void visit(common::Id &node);

    void visit(common::Int &node);

    llvm::AllocaInst *CreateAlloca(const string &name);

    llvm::Type *get_type(common::Types type);
};
}

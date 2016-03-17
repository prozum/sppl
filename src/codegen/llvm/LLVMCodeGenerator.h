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
    llvm::BasicBlock *cur_pattern_block;
    llvm::BasicBlock *cur_case_block;

private:
    std::vector<llvm::Argument *> Arguments;
    std::vector<std::vector<llvm::BasicBlock *>> PatternBlocks;
    std::map<std::string, llvm::Value *> ContextValues;
    llvm::Value *cur_val;
    llvm::BasicBlock *cur_error_block;
    size_t cur_case_id;
    size_t last_case_id;
    Context ctx;

    void visit(common::Id &node);

    void visit(common::Int &node);

    llvm::AllocaInst *CreateAlloca(const string &name);

    llvm::Type *get_type(common::Types type);

    llvm::Value *compare(llvm::Value *val1, llvm::Value *val2);
};
}

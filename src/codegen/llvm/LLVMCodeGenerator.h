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

    llvm::IRBuilder<> Builder;
    unique_ptr<llvm::Module> Module;
    std::map<std::string, llvm::Value *> ContextValues;
    llvm::Function *GreateAnonymousFunction(common::Expr *expr);

private:
    llvm::Function *cur_func;
    llvm::Value *cur_val;
    llvm::BasicBlock *cur_pattern_block;
    llvm::BasicBlock *cur_case_block;
    llvm::BasicBlock *cur_error_block;
    llvm::BasicBlock *cur_ret_block;
    llvm::PHINode *cur_phi_node;
    std::vector<llvm::Argument *> arguments;
    size_t cur_case_id;
    size_t last_case_id;
    Context ctx;

    void visit(common::Function &node);
    void visit(common::Case &node);

    void visit(common::Int &node);
    void visit(common::Float &node);
    void visit(common::Bool &node);
    void visit(common::Char &node);
    void visit(common::String &node);

    void visit(common::Add &node);
    void visit(common::Sub &node);
    void visit(common::Mul &node);
    void visit(common::Div &node);
    void visit(common::Mod &node);

	void visit(common::Equal &node);
	void visit(common::NotEqual &node);
	void visit(common::Lesser &node);
	void visit(common::Greater &node);
	void visit(common::LesserEq &node);
	void visit(common::GreaterEq &node);

	void visit(common::Id &node);
    void visit(common::Call &node);
	void visit(common::Par &node);

    llvm::Type *get_type(common::Types type);

    llvm::Value *compare(llvm::Value *val1, llvm::Value *val2);

};
}

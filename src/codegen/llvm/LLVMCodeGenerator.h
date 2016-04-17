#pragma once
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/IR/TypeBuilder.h>

#include <llvm/Support/raw_os_ostream.h>

#include <unordered_map>
#include <iostream>

#include "CodeGenerator.h"
#include "Driver.h"

using namespace std;
using namespace parser;

namespace codegen {

    enum Context {
        PATTERN,
        EXPR,
    };

class LLVMCodeGenerator : public CodeGenerator {
  public:
    LLVMCodeGenerator(parser::Driver &driver);

    void visit(common::Program &node);

    llvm::IRBuilder<> Builder;
    unique_ptr<llvm::Module> Module;
    std::map<std::string, llvm::Value *> ContextValues;

	string ModuleString();

	std::unordered_map<common::Type, llvm::StructType *> TupleTypes;
	std::unordered_map<common::Type, llvm::StructType *> ListTypes;
    std::unordered_map<common::Type, llvm::FunctionType *> FuncTypes;
	std::map<std::string, llvm::Function *> Functions;

    llvm::Function *CurFunc;
    llvm::Value *CurVal;
    llvm::BasicBlock *CurPatternBlock;
    llvm::BasicBlock *CurCaseBlock;
    llvm::BasicBlock *CurErrorBlock;
    llvm::BasicBlock *CurRetBlock;
    llvm::PHINode *CurPhiNode;
    std::vector<llvm::Argument *> Arguments;
    size_t CurCaseId;
    size_t LastCaseId;
    Context Ctx;

    void visit(common::Function &node);
    void visit(common::Case &node);

    void visit(common::Int &node);
    void visit(common::Float &node);
    void visit(common::Bool &node);
    void visit(common::Char &node);
    void visit(common::String &node);
	void visit(common::Tuple &node);
	void visit(common::List &node);

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

    llvm::Type *getType(common::Type node_type, bool ptr = false);
	llvm::StructType *getTupleType(common::Type type);
	llvm::StructType *getListType(common::Type type);
    llvm::FunctionType *getFuncType(common::Type type);

    llvm::Value *compare(llvm::Value *val1, llvm::Value *val2);

};
}

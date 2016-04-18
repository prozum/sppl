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

        void visit(common::Function &Node);
        void visit(common::Case &Node);

        void visit(common::IntPattern &Node);
        void visit(common::FloatPattern &Node);
        void visit(common::Bool &Node);
        void visit(common::CharPattern &Node);
        void visit(common::String &Node);
    	void visit(common::TupleExpression &Node);
    	void visit(common::ListExpression &Node);

        void visit(common::Add &Node);
        void visit(common::Sub &Node);
        void visit(common::Mul &Node);
        void visit(common::Div &Node);
        void visit(common::Mod &Node);

    	void visit(common::Equal &Node);
    	void visit(common::NotEqual &Node);
    	void visit(common::Lesser &Node);
    	void visit(common::Greater &Node);
    	void visit(common::LesserEq &Node);
    	void visit(common::GreaterEq &Node);

    	void visit(common::IdPattern &Node);
        void visit(common::Call &Node);
    	void visit(common::Par &Node);

        llvm::Type *getType(common::Type Ty, bool Ptr = false);
    	llvm::StructType *getTupleType(common::Type Ty);
    	llvm::StructType *getListType(common::Type Ty);
        llvm::FunctionType *getFuncType(common::Type Ty);

	    llvm::Value *compare(llvm::Value *Val1, llvm::Value *Val2);
	};
}

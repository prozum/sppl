#pragma once
#include "CodeGenerator.h"
#include "Driver.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/IR/TypeBuilder.h>

#include <llvm/Support/raw_os_ostream.h>

#include <unordered_map>
#include <iostream>


using namespace std;
using namespace parser;
using namespace llvm;

namespace codegen {

    class LLVMCodeGenerator : public CodeGenerator {
      public:
        LLVMCodeGenerator(parser::Driver &driver);


        llvm::IRBuilder<> Builder;
        unique_ptr<llvm::Module> Module;
        std::map<std::string, llvm::Value *> CtxVals;

    	std::unordered_map<common::Type, llvm::StructType *> TupleTypes;
    	std::unordered_map<common::Type, llvm::StructType *> ListTypes;
        std::unordered_map<common::Type, llvm::FunctionType *> FuncTypes;

        /*
        llvm::BasicBlock *CurPatternBlock;
        llvm::BasicBlock *CurCaseBlock;
        */

        llvm::Value *CurVal;

        llvm::Function *CurFunc;
        llvm::BasicBlock *CurEntry;
        llvm::BasicBlock *CurErrBlock;
        llvm::BasicBlock *CurRetBlock;
        llvm::PHINode *CurPhiNode;

        vector<Argument *> Args;
        vector<Argument *>::const_iterator CurArg;

        vector<unique_ptr<Case>>::const_iterator CurCase;
        vector<BasicBlock *> CaseBlocks;
        vector<BasicBlock *>::const_iterator CurCaseBlock;
        vector<BasicBlock *>::const_iterator LastCaseBlock;

        vector<unique_ptr<Pattern>>::const_iterator CurPat;
        vector<vector<BasicBlock *>> PatVecBlocks;
        vector<vector<BasicBlock *>>::const_iterator CurPatVecBlock;
        vector<BasicBlock *>::const_iterator CurPatBlock;
        vector<BasicBlock *>::const_iterator LastPatBlock;

		string ModuleString();

        void visit(common::Program &node);
        void visit(common::Function &Node);
        void visit(common::Case &Node);

		void visit(common::IdPattern &Node);
        void visit(common::IntPattern &Node);
        void visit(common::FloatPattern &Node);
        void visit(common::BoolPattern &Node);
        void visit(common::CharPattern &Node);

        void visit(common::Add &Node);
        void visit(common::Sub &Node);
        void visit(common::Mul &Node);
        void visit(common::Div &Node);
        void visit(common::Mod &Node);

		void visit(common::And &Node);
		void visit(common::Or &Node);
    	void visit(common::Equal &Node);
    	void visit(common::NotEqual &Node);
    	void visit(common::Lesser &Node);
    	void visit(common::Greater &Node);
    	void visit(common::LesserEq &Node);
    	void visit(common::GreaterEq &Node);

		void visit(common::IdExpr &Node);
        void visit(common::IntExpr &Node);
        void visit(common::FloatExpr &Node);
        void visit(common::BoolExpr &Node);
        void visit(common::CharExpr &Node);
		void visit(common::StringExpr &Node);
    	void visit(common::TupleExpr &Node);
    	void visit(common::ListExpr &Node);
        void visit(common::CallExpr &Node);
    	void visit(common::ParExpr &Node);

        llvm::Type *getType(common::Type Ty, bool Ptr = false);
    	llvm::StructType *getTupleType(common::Type Ty);
    	llvm::StructType *getListType(common::Type Ty);
        llvm::FunctionType *getFuncType(common::Type Ty);
	};
}

#pragma once
#include "CodeGenerator.h"
#include "Driver.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/IR/TypeFinder.h>

#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Bitcode/ReaderWriter.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Transforms/Scalar.h>

#include <iostream>
#include <unordered_map>
#include <ctype.h>

namespace codegen {
class LLVMCodeGen : public parser::CodeGenerator {
  public:
    LLVMCodeGen(parser::Driver &Drv);

    // LLVM Context
    // Only one context is used, as the LLVM code generator is single threaded
    llvm::LLVMContext &Ctx;

    // LLVM IR Builder
    llvm::IRBuilder<> Builder;

    // Target Machine
    // Used to get information about the target machine
    std::unique_ptr<llvm::TargetMachine> Machine;

    // Data Layout
    // Stores information about data layout for the target machine
    std::unique_ptr<llvm::DataLayout> DataLayout;

    // Module
    // Used to store functions and global variables
    std::unique_ptr<llvm::Module> Module;
    std::unique_ptr<llvm::Module> ModuleHeader;

    // PassManager
    // Used to perform optimization on module
    std::unique_ptr<llvm::legacy::PassManager> PassMgr;

    // Identifer Values
    std::map<std::string, llvm::Value *> IdVals;

    // Tail recursion for CurFunc
    bool TailRec = false;

    // Get data alignment for target machine
    unsigned getAlignment(common::Type Ty);

    // Get IR string method
    std::string ModuleString();

    // Initialize LLVM to target running machine
    static void initLLVM() {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
    }

private:
    // Stream to output LLVM errors
    llvm::raw_os_ostream MsgOut;

    // Maps for type caching
    std::unordered_map<common::Type, llvm::StructType *> TupleTypes;
    std::unordered_map<common::Type, llvm::StructType *> ListTypes;
    std::unordered_map<common::Type, llvm::FunctionType *> FuncTypes;
    std::unordered_map<common::Type, llvm::GlobalVariable *> RuntimeTypes;

    // Type constants
    llvm::IntegerType *Int1;
    llvm::IntegerType *Int8;
    llvm::IntegerType *Int32;
    llvm::PointerType *VoidPtr;

    llvm::IntegerType *Int;
    llvm::Type *Float;

    llvm::StructType *UnionType;
    llvm::StructType *RuntimeType;
    llvm::FunctionType *MainType;

    // Helper functions
    std::unordered_map<std::string, llvm::FunctionType *> InternFuncs;
    std::unordered_map<std::string, llvm::Function *> InternFuncDecls;

    // Current state variables
    llvm::Value *CurVal = nullptr;
    llvm::Function *CurFunc = nullptr;
    std::vector<llvm::Value *> Args;
    std::vector<llvm::Value *>::const_iterator CurArg;
    std::vector<std::unique_ptr<common::Case>>::const_iterator CurCase;
    std::vector<std::unique_ptr<common::Pattern>>::const_iterator CurPat;
    llvm::BasicBlock *CurCaseBlock;
    llvm::BasicBlock *CurPatBlock;

    // Basic blocks
    llvm::BasicBlock *Entry = nullptr;
    llvm::BasicBlock *ErrBlock = nullptr;
    llvm::BasicBlock *RetBlock = nullptr;
    llvm::PHINode *RetPhiNode = nullptr;
    llvm::BasicBlock *NextBlock = nullptr;

    // Bool used to find first block in function
    bool FirstBlock;

    // Prefixes
    std::vector<std::pair<std::string,int>> Prefixes;

    // Visit methods
    void visit(common::Program &node);
    void visit(common::Function &Node);
    void visit(common::Case &Node);

    void visit(common::IdPattern &Node);
    void visit(common::IntPattern &Node);
    void visit(common::FloatPattern &Node);
    void visit(common::BoolPattern &Node);
    void visit(common::CharPattern &Node);
    void visit(common::TuplePattern &Node);
    void visit(common::StringPattern &Node);
    void visit(common::ListPattern &Node);
    void visit(common::ParPattern &Node);
    void visit(common::WildPattern &Node);
    void visit(common::ListSplit &Node);

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
    void visit(common::To &Node);
    void visit(common::ListAdd &Node);
    void visit(common::DoExpr &Node);
    void visit(common::Assosiate &Node);

    void visit(common::Negative &Node);
    void visit(common::Not &Node);
    void visit(common::UnPrint &Node);

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

    void createModule();

    // Standard library methods
    void initStdLib();
    void addInternFunc(std::string FuncName, llvm::FunctionType *Ty);
    llvm::Function *getInternFunc(std::string FuncName);
    void createArgFunc();
    void createPrintFunc();
    void createPrintTupleFunc();
    void CreatePrintListFunc();
    void CreatePrintSignatureFunc();

    // Utility methods
    void splitCaseBlock(std::string Name);
    llvm::Value *createListNode(common::Type Type, llvm::Value *Data, llvm::Value *NextNode, llvm::BasicBlock *Block,
                                bool Const = false);
    llvm::Instruction *createMalloc(llvm::Type *Type, llvm::BasicBlock *Block);
    llvm::Instruction *createMalloc(llvm::Value *Size, llvm::BasicBlock *Block);
    void createPrint(llvm::Value *Data, common::Type Ty);

    // Type methods
    void initTypes();
    llvm::Type *getType(common::Type Ty);
    llvm::StructType *getTupleType(common::Type Ty);
    llvm::StructType *getListType(common::Type Ty);
    llvm::GlobalVariable *getRuntimeType(common::Type Ty);
    llvm::FunctionType *getFuncType(common::Type Ty);

    // Prefix methods
    void addPrefix(std::string Prefix, bool Numbered = true);
    void delPrefix();
    std::string getPrefix();
    std::string getPrefix(std::string Str);
    void stepPrefix();
};
}

#pragma once
#include "CodeGenerator.h"
#include "Driver.h"

#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/PassManager.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/IR/ModuleSlotTracker.h>

#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/TargetSelect.h>

#include <llvm/Bitcode/ReaderWriter.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <llvm/Analysis/Passes.h>
#include <llvm/Transforms/Scalar.h>

#include <iostream>
#include <unordered_map>
#include <ctype.h>

namespace codegen {

#define SPPL_DECL "sppl_decl"

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

    // PassManager
    // Used to perform optimization on module
    std::unique_ptr<llvm::legacy::PassManager> PassMgr;

    // Identifier Values
    std::map<std::string, llvm::Value *> IdVals;

    // Tail recursion for CurFunc
    bool TailRec = false;

    // Methods used for JIT
    unsigned getAlignment(common::Type Ty);
    std::unique_ptr<llvm::Module> getStdModule();

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
    std::unordered_map<common::Type, llvm::GlobalVariable *> RunTypes;

    // Type constants
    llvm::IntegerType *Int1;
    llvm::IntegerType *Int8;
    llvm::IntegerType *Int32;
    llvm::PointerType *VoidPtr;

    llvm::IntegerType *Int;
    llvm::Type *Float;

    llvm::FunctionType *MainType;
    llvm::StructType *RunType;
    llvm::StructType *UnionType;
    llvm::StructType *ListType;

    // Standard functions
    std::unordered_map<std::string, llvm::FunctionType *> StdFuncs;
    std::unordered_map<std::string, llvm::Function *> StdFuncDecls;

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
    llvm::PHINode *RetPhi = nullptr;
    llvm::BasicBlock *NextBlock = nullptr;

    // Bool used to find first block in function
    bool FirstBlock = false;
    // Bool used to indicate concat to lists
    bool Concat = false;

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
    void visit(common::Concat &Node);

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
    llvm::Module *getStdLib();
    void setTriple();
    void addStdFunc(std::string FuncName, common::Type Ty, bool Decl = false);
    void addStdFunc(std::string FuncName, llvm::FunctionType *Ty, bool Decl = false);
    llvm::Function *getStdFunc(std::string FuncName);
    void createArgFunc();
    void createPrintFunc();

    // Utility methods
    llvm::Value *createListNode(common::Type Type, llvm::Value *Data, llvm::Value *NextNode, llvm::BasicBlock *Block,
                                bool Const = false);
    llvm::Instruction *createMalloc(llvm::Type *Type, llvm::BasicBlock *Block);
    llvm::Instruction *createMalloc(llvm::Value *Size, llvm::BasicBlock *Block);
    void createPrint(llvm::Value *Data, common::Type Ty);

    // LLVM type methods
    void initTypes();
    llvm::Type *getLLVMType(common::Type Ty);
    llvm::StructType *getLLVMTupleType(common::Type Ty);
    llvm::StructType *getLLVMListType(common::Type Ty);
    llvm::GlobalVariable *getRuntimeType(common::Type Ty);
    llvm::FunctionType *getLLVMFuncType(common::Type Ty);

    // SPPL type methods
    common::Type getType(llvm::Type *Ty);
    common::Type getFuncType(llvm::FunctionType *FuncTy);
    common::Type getFuncType(llvm::GlobalVariable *FuncRunTy);

    // Prefix methods
    void addPrefix(std::string Prefix, bool Numbered = true);
    void delPrefix();
    std::string getPrefix();
    std::string getPrefix(std::string Str);
    void stepPrefix();
};
}

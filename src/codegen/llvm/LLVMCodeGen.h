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

#include <iostream>
#include <unordered_map>

namespace codegen {
class LLVMCodeGen : public parser::CodeGenerator {
  public:
    LLVMCodeGen(parser::Driver &Drv);

    llvm::LLVMContext &Ctx;
    llvm::IRBuilder<> Builder;

    std::unique_ptr<llvm::TargetMachine> Machine;
    std::unique_ptr<llvm::DataLayout> DataLayout;
    std::unique_ptr<llvm::Module> Module;

    std::map<std::string, llvm::Value *> PatVals;
    bool TailRec = false;
    bool Runtime = false;

    unsigned getAlignment(common::Type Ty);
    std::string ModuleString();

    static void initLLVM() {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
    }

private:
    llvm::raw_os_ostream RawOut;

    std::unordered_map<common::Type, llvm::StructType *> TupleTypes;
    std::unordered_map<common::Type, llvm::StructType *> ListTypes;
    std::unordered_map<common::Type, llvm::FunctionType *> FuncTypes;

    // Type Constants
    llvm::Type *Int1;
    llvm::Type *Int8;
    llvm::Type *Int32;
    llvm::Type *Int64;
    llvm::Type *Double;
    llvm::PointerType *VoidPtr;
    llvm::FunctionType *MainType;

    llvm::Value *CurVal = nullptr;
    llvm::Function *CurFunc = nullptr;

    llvm::BasicBlock *Entry = nullptr;
    llvm::BasicBlock *ErrBlock = nullptr;
    llvm::BasicBlock *CaseRetBlock = nullptr;
    llvm::PHINode *CasePhiNode = nullptr;
    llvm::BasicBlock *NextPatBlock = nullptr;

    std::vector<llvm::Value *> Args;
    std::vector<llvm::Value *>::const_iterator CurArg;

    std::vector<std::unique_ptr<common::Case>>::const_iterator CurCase;
    std::vector<std::unique_ptr<common::Pattern>>::const_iterator CurPat;
    llvm::BasicBlock *CurCaseBlock;
    llvm::BasicBlock *CurPatBlock;

    std::vector<std::pair<std::string,int>> Prefixes;

    bool FirstBlock;

    void visit(common::Program &node);
    void visit(common::Function &Node);
    void visit(common::Case &Node);

    void visit(common::IdPattern &Node);
    void visit(common::IntPattern &Node);
    void visit(common::FloatPattern &Node);
    void visit(common::BoolPattern &Node);
    void visit(common::CharPattern &Node);
    void visit(common::TuplePattern &Node);
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

    void visit(common::Negative &Node);
    void visit(common::Not &Node);

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

    llvm::Value *CreateListNode(common::Type Type, llvm::Value *Data, llvm::Value *NextNode, llvm::BasicBlock *Block, bool Runtime = false);
    llvm::Instruction *CreateMalloc(llvm::Type *Type, llvm::BasicBlock *Block);
    llvm::Instruction *CreateMalloc(llvm::Value *Size, llvm::BasicBlock *Block);
    llvm::Function *CreateMain();

    llvm::Type *getType(common::Type Ty);
    llvm::StructType *getTupleType(common::Type Ty);
    llvm::StructType *getListType(common::Type Ty);
    llvm::FunctionType *getFuncType(common::Type Ty);

    void addPrefix(std::string Prefix);
    void delPrefix();
    std::string getPrefix();
    void stepPrefix();
};
}

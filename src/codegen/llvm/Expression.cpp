#include "LLVMCodeGenerator.h"

using namespace codegen;
using namespace llvm;

void LLVMCodeGenerator::visit(common::FloatExpr &Node) {
    CurVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
}

void LLVMCodeGenerator::visit(common::IntExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::BoolExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::StringExpr &Node) {
    CurVal = Builder.CreateGlobalString(Node.Val, CurFunc->getName() + ".str");
}

void LLVMCodeGenerator::visit(common::CharExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::IdExpr &Node) {
    // Pattern value
    CurVal = ContextValues[Node.Val];
    if (CurVal)
        return;

    // Internal module
    CurVal = Module->getFunction(Node.Val);
    if (CurVal)
        return;

    // External module
    if (Drv.Global.Decls.count(Node.Val))
        CurVal = llvm::Function::Create(getFuncType(Drv.Global.Decls[Node.Val]), llvm::Function::ExternalLinkage, Node.Val, Module.get());
    if (CurVal)
        return;

    throw runtime_error("This should not happen");
}

void LLVMCodeGenerator::visit(common::ParExpr &Node) {
    Node.Child->accept(*this);
}

void LLVMCodeGenerator::visit(common::TupleExpr &Node) {
    std::vector<llvm::Constant *> TmpVec;

    for (auto &Element : Node.Elements) {
        Element->accept(*this);
        TmpVec.push_back((Constant *)CurVal);
    }

    ArrayRef<Constant *> TupleVal(TmpVec);

    auto ConstVal = ConstantStruct::get(getTupleType(Node.RetTy), TupleVal);
    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true, GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGenerator::visit(common::ListExpr &Node) {
    std::vector<llvm::Constant *> TmpVec;

    for (auto &Element : Node.Elements) {
        Element->accept(*this);
        TmpVec.push_back(dynamic_cast<Constant *>(CurVal));
    }

    ArrayRef<Constant *> ListData(TmpVec);

    auto ListType = ArrayType::get(getType(Node.RetTy), Node.Elements.size());
    auto ConstVal = ConstantArray::get(ListType, ListData);

    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true, GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGenerator::visit(common::CallExpr &Node) {
    Node.Callee->accept(*this);
    auto Callee = CurVal;

    std::vector<Value *> Args;
    for (auto &Arg : Node.Args) {
        Arg->accept(*this);
        Args.push_back(CurVal);
    }

    CurVal = Builder.CreateCall(Callee, Args, "calltmp");
}

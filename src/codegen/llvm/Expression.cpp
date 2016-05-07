#include "LLVMCodeGen.h"

using namespace std;
using namespace codegen;
using namespace llvm;

void LLVMCodeGen::visit(common::FloatExpr &Node) {
    CurVal = ConstantFP::get(getType(Node.RetTy), Node.Val);
}

void LLVMCodeGen::visit(common::IntExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGen::visit(common::BoolExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGen::visit(common::StringExpr &Node) {
    auto GlobalVar = Builder.CreateGlobalString(Node.Val, CurFunc->getName() + ".str");

    CurVal = CreateList(Node.RetTy, GlobalVar, ConstantInt::get(Int32, Node.Val.size()), *CurCaseBlock);
}

void LLVMCodeGen::visit(common::CharExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGen::visit(common::IdExpr &Node) {
    // Pattern value
    CurVal = CtxVals[Node.Val];
    if (CurVal)
        return;

    // Internal module
    CurVal = Module->getFunction(Node.Val);
    if (CurVal)
        return;

    // External module
    if (Drv.Global.Decls.count(Node.Val))
        CurVal = llvm::Function::Create(getFuncType(Drv.Global.Decls[Node.Val]),
                                        llvm::Function::ExternalLinkage,
                                        Node.Val, Module.get());
    if (CurVal)
        return;

    assert(0 && "Something went wrong in the TypeChecker");
}

void LLVMCodeGen::visit(common::ParExpr &Node) {
    Node.Child->accept(*this);
}

void LLVMCodeGen::visit(common::TupleExpr &Node) {
    std::vector<llvm::Constant *> TmpVec;

    for (auto &Element : Node.Elements) {
        Element->accept(*this);
        TmpVec.push_back((Constant *)CurVal);
    }

    ArrayRef<Constant *> TupleVal(TmpVec);

    auto ConstVal = ConstantStruct::get(getTupleType(Node.RetTy), TupleVal);
    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGen::visit(common::ListExpr &Node) {
    // Create types
    auto ArrayType = ArrayType::get(getType(Node.RetTy.Subtypes.front()), Node.Elements.size());

    // Malloc list data
    auto DataMalloc = CreateMalloc(ArrayType, *CurCaseBlock);
    auto DataCast = Builder.CreateBitCast(DataMalloc, PointerType::getUnqual(ArrayType));

    // Create global list constant
    for (size_t i = 0; i < Node.Elements.size(); ++i) {
        Node.Elements[i]->accept(*this);
        auto GEP = Builder.CreateStructGEP(ArrayType, DataCast, i);
        Builder.CreateStore(CurVal, GEP);
    }

    /*
    // Malloc list data
    auto DataMalloc = CreateMalloc(ArrayType, *CurCaseBlock);

    // Memcpy
    auto Size = DataLayout.getPointerTypeSize(ArrayType);
    Builder.CreateMemCpy(DataMalloc, GlobalVal, Size, 1);

    // Malloc list container
    auto ListMalloc = CreateMalloc(ListType, *CurCaseBlock);
    auto ListCast = Builder.CreateBitCast(ListMalloc, ListPtrType);

    // Set list length
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 0);
    Builder.CreateStore(ConstantInt::get(Int32, Node.Elements.size()), CurVal);

    // Set list data
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 1);
    auto DataCast = Builder.CreateBitCast(DataMalloc, PointerType::getUnqual(ArrayType::get(getType(Node.RetTy.Subtypes[0]), 0)));
    Builder.CreateStore(DataCast, CurVal);
    */
    auto List = CreateList(Node.RetTy, DataCast, ConstantInt::get(Int32, Node.Elements.size()), *CurCaseBlock);

    // Set return value
    auto ListPtrType = getType(Node.RetTy);
    auto RetVal = Builder.CreateAlloca(ListPtrType, nullptr, "allocatmp");
    Builder.CreateStore(List, RetVal);
    CurVal = Builder.CreateLoad(ListPtrType, RetVal, "loadtmp");
}



void LLVMCodeGen::visit(common::CallExpr &Node) {
    Node.Callee->accept(*this);
    auto Callee = CurVal;

    std::vector<Value *> Args;
    for (auto &Arg : Node.Args) {
        Arg->accept(*this);
        Args.push_back(CurVal);
    }

    auto Call = Builder.CreateCall(Callee, Args, "calltmp");

    // Set tail recursion state
    Call->setTailCall(TailRec);

    CurVal = Call;
}



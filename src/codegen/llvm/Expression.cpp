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

    //CurVal = CreateList(Node.RetTy, GlobalVar, ConstantInt::get(Int32, Node.Val.size()), *CaseBlock);
    assert(0);
}

void LLVMCodeGen::visit(common::CharExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGen::visit(common::IdExpr &Node) {
    // Pattern value
    CurVal = PatVals[Node.Val];
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
    std::vector<llvm::Constant *> TupleVal;

    for (auto &Element : Node.Elements) {
        Element->accept(*this);
        TupleVal.push_back((Constant *)CurVal);
    }

    auto ConstVal = ConstantStruct::get(getTupleType(Node.RetTy), TupleVal);
    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGen::visit(common::ListExpr &Node) {
    // Create types
    /*
    auto ArrayType = ArrayType::get(getType(Node.RetTy.Subtypes.front()), Node.Elements.size());
    */
    //auto ListType = getListType(Node.RetTy);


    // Malloc list data
    /*
    auto DataMalloc = CreateMalloc(ArrayType, *CaseBlock);
    auto DataCast = Builder.CreateBitCast(DataMalloc, PointerType::getUnqual(ArrayType));
     */

    // Create global list constant
    Value *ListNode = nullptr;
    for (auto Element = Node.Elements.rbegin(); Element < Node.Elements.rend(); ++Element) {

        (*Element)->accept(*this);
        ListNode = CreateListNode(Node.RetTy, CurVal, ListNode, *CaseBlock);
        //auto GEP = Builder.CreateStructGEP(ArrayType, DataCast, i);
        //Builder.CreateStore(CurVal, GEP);
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
    //auto List = CreateList(Node.RetTy, DataCast, ConstantInt::get(Int32, Node.Elements.size()), *CaseBlock);

    // Set return value
    auto ListPtrType = getType(Node.RetTy);
    auto RetVal = Builder.CreateAlloca(ListPtrType, nullptr, "allocatmp");
    Builder.CreateStore(ListNode, RetVal);
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
    Call->setCallingConv(CallingConv::Fast);
    Call->setTailCall(TailRec);

    CurVal = Call;
}



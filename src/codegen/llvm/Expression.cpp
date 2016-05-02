#include "LLVMCodeGenerator.h"

using namespace std;
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
    auto GlobalVar = Builder.CreateGlobalString(Node.Val, CurFunc->getName() + ".str");

    CurVal = CreateList(Node.RetTy, GlobalVar, Node.Val.size());
}

void LLVMCodeGenerator::visit(common::CharExpr &Node) {
    CurVal = ConstantInt::get(getType(Node.RetTy), (uint64_t)Node.Val);
}

void LLVMCodeGenerator::visit(common::IdExpr &Node) {
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
    CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                GlobalVariable::ExternalLinkage, ConstVal);
}

void LLVMCodeGenerator::visit(common::ListExpr &Node) {
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
    auto List = CreateList(Node.RetTy, DataCast, Node.Elements.size());

    // Set return value
    auto ListPtrType = getType(Node.RetTy);
    auto RetVal = Builder.CreateAlloca(ListPtrType, nullptr, "allocatmp");
    Builder.CreateStore(List, RetVal);
    CurVal = Builder.CreateLoad(ListPtrType, RetVal, "loadtmp");
}

Value *LLVMCodeGenerator::CreateList(common::Type Type, Value *Data, size_t Size)
{
    auto ListType = getListType(Type);
    auto ListPtrType = getType(Type);

    // Malloc list container
    auto ListMalloc = CreateMalloc(ListType, *CurCaseBlock);
    auto ListCast = Builder.CreateBitCast(ListMalloc, ListPtrType);

    // Set list length
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 0);
    Builder.CreateStore(ConstantInt::get(Int32, Size), CurVal);

    // Set list data
    CurVal = Builder.CreateStructGEP(ListType, ListCast, 1);
    auto DataCast = Builder.CreateBitCast(Data, PointerType::getUnqual(ArrayType::get(getType(Type.Subtypes.front()), 0)));
    Builder.CreateStore(DataCast, CurVal);

    return ListMalloc;
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

Instruction *LLVMCodeGenerator::CreateMalloc(llvm::Type *Type, BasicBlock *Block)
{
    auto Size = DataLayout.getPointerTypeSize(Type);
    auto AllocSize = ConstantInt::get(Int32, APInt(32, Size));
    auto Malloc = CallInst::CreateMalloc(Block,
                                         Int32, Type, AllocSize,
                                         nullptr, nullptr, "malloccall");
    Block->getInstList().push_back(Malloc);

    return Malloc;
}

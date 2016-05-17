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
    Value *ListNode = nullptr;
    for (auto Char = Node.Val.rbegin(); Char < Node.Val.rend(); ++Char) {
        CurVal = ConstantInt::get(Int, (uint64_t)*Char);
        ListNode = CreateListNode(Node.RetTy, CurVal, ListNode, Builder.GetInsertBlock(), true);
    }
    CurVal = ListNode;
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
    auto TupleType = getTupleType(Node.RetTy);
    auto TuplePtrType = getType(Node.RetTy);

    if (Node.Const) {
        std::vector<llvm::Constant *> TupleVal;
        for (auto &Element : Node.Elements) {
            Element->accept(*this);
            TupleVal.push_back(static_cast<Constant *>(CurVal));
        }

        auto ConstVal = ConstantStruct::get(TupleType, TupleVal);
        CurVal = new GlobalVariable(*Module.get(), ConstVal->getType(), true,
                                    GlobalVariable::ExternalLinkage, ConstVal);
        return;
    }

    auto TupleMalloc = CreateMalloc(TupleType, CurCaseBlock);
    auto Tuple = Builder.CreateBitCast(TupleMalloc, TuplePtrType);

    for (size_t i = 0; i < Node.Elements.size(); ++i) {
        Node.Elements[i]->accept(*this);

        auto GEP = Builder.CreateStructGEP(TupleType, Tuple, i);
        Builder.CreateStore(CurVal, GEP);
    }

    CurVal = Tuple;
}

void LLVMCodeGen::visit(common::ListExpr &Node) {
    Value *ListNode = nullptr;
    for (auto Element = Node.Elements.rbegin(); Element < Node.Elements.rend(); ++Element) {
        (*Element)->accept(*this);
        ListNode = CreateListNode(Node.RetTy, CurVal, ListNode, Builder.GetInsertBlock(), Node.Const);
    }

    if (!ListNode)
        ListNode = ConstantPointerNull::get(static_cast<PointerType *>(getType(Node.RetTy)));

    CurVal = ListNode;
}



void LLVMCodeGen::visit(common::CallExpr &Node) {
    Node.Callee->accept(*this);
    auto Callee = CurVal;

    std::vector<Value *> Args;
    for (auto &Arg : Node.Args) {
        Arg->accept(*this);
        Args.push_back(CurVal);
    }

    auto Call = Builder.CreateCall(Callee, Args);

    // Set tail recursion state
    Call->setCallingConv(CallingConv::Fast);
    Call->setTailCall(TailRec);

    CurVal = Call;
}

void LLVMCodeGen::visit(common::ListAdd &Node) {
    Node.Left->accept(*this);
    auto Element = CurVal;

    Node.Right->accept(*this);
    auto List = CurVal;

    CurVal = CreateListNode(Node.RetTy, Element, List, Builder.GetInsertBlock());
}

void LLVMCodeGen::visit(common::DoExpr &Node) {
    for (auto &Expr : Node.Exprs) {
        Expr->accept(*this);
    }
}

void LLVMCodeGen::visit(common::Assosiate &Node) {
    Node.Child->accept(*this);

    PatVals[Node.Id] = CurVal;
}

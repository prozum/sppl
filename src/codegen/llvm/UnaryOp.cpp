#include "LLVMCodeGen.h"

using namespace codegen;

void LLVMCodeGen::visit(common::Negative &Node) {
    Node.Child->accept(*this);

    assert(Node.Child->RetTy.Id == common::TypeId::INT || Node.Child->RetTy.Id == common::TypeId::FLOAT );

    if (Node.Child->RetTy.Id == common::TypeId::INT) {
        CurVal = Builder.CreateNeg(CurVal, "negtmp");
    }
    else {
        CurVal = Builder.CreateFNeg(CurVal, "fnegtmp");
    }
}

void LLVMCodeGen::visit(common::Not &Node) {
    Node.Child->accept(*this);

    assert(Node.Child->RetTy.Id == common::TypeId::BOOL);

    CurVal = Builder.CreateNot(CurVal, "nottmp");
}

void LLVMCodeGen::visit(common::UnPrint &Node) {
    Node.Child->accept(*this);

    CreatePrint(CurVal, Node.RetTy);
}
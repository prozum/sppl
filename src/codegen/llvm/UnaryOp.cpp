#include "LLVMCodeGenerator.h"

using namespace codegen;

void LLVMCodeGenerator::visit(common::Negative &Node) {
    Node.Child->accept(*this);

    assert(Node.Child->RetTy.Id == common::TypeId::INT || Node.Child->RetTy.Id == common::TypeId::FLOAT );

    if (Node.Child->RetTy.Id == common::TypeId::INT) {
        CurVal = Builder.CreateNeg(CurVal, "negtmp");
    }
    else {
        CurVal = Builder.CreateFNeg(CurVal, "fnegtmp");
    }
}

void LLVMCodeGenerator::visit(common::Not &Node) {
    Node.Child->accept(*this);

    assert(Node.Child->RetTy.Id == common::TypeId::BOOL);

    CurVal = Builder.CreateNot(CurVal, "nottmp");
}
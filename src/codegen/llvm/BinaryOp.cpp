#include "LLVMCodeGenerator.h"

using namespace codegen;

void LLVMCodeGenerator::visit(common::Add &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFAdd(left, right, "addtmp");
    } else {
        CurVal = Builder.CreateAdd(left, right, "addtmp");
    }
}

void LLVMCodeGenerator::visit(common::Sub &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFSub(left, right, "subtmp");
    } else {
        CurVal = Builder.CreateSub(left, right, "subtmp");
    }
}

void LLVMCodeGenerator::visit(common::Mul &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFMul(left, right, "multmp");
    } else {
        CurVal = Builder.CreateMul(left, right, "multmp");
    }
}

void LLVMCodeGenerator::visit(common::Div &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFDiv(left, right, "divtmp");
    } else {
        CurVal = Builder.CreateSDiv(left, right, "divtmp");
    }
}

void LLVMCodeGenerator::visit(common::Mod &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFRem(left, right, "modtmp");
    } else {
        CurVal = Builder.CreateSRem(left, right, "modtmp");
    }
}


void LLVMCodeGenerator::visit(common::And &Node) {
    assert(Node.Left->RetTy.Id == common::TypeId::BOOL && Node.Right->RetTy.Id == common::TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateAnd(Left, Right, "andtmp");
}

void LLVMCodeGenerator::visit(common::Or &Node) {
    assert(Node.Left->RetTy.Id == common::TypeId::BOOL && Node.Right->RetTy.Id == common::TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateOr(Left, Right, "ortmp");
}

void LLVMCodeGenerator::visit(common::Equal &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOEQ(left, right, "eqtmp");
    } else {
        CurVal = Builder.CreateICmpEQ(left, right, "eqtmp");
    }
}

void LLVMCodeGenerator::visit(common::NotEqual &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpONE(left, right, "neqtmp");
    } else {
        CurVal = Builder.CreateICmpNE(left, right, "neqtmp");
    }
}

void LLVMCodeGenerator::visit(common::Lesser &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOLT(left, right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSLT(left, right, "lttmp");
    }
}

void LLVMCodeGenerator::visit(common::Greater &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOGT(left, right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSGT(left, right, "lttmp");
    }
}

void LLVMCodeGenerator::visit(common::LesserEq &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOLE(left, right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSLE(left, right, "lttmp");
    }
}

void LLVMCodeGenerator::visit(common::GreaterEq &Node) {
    Node.Left->accept(*this);
    auto left = CurVal;
    Node.Right->accept(*this);
    auto right = CurVal;

    if ( Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOGE(left, right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSGE(left, right, "lttmp");
    }
}


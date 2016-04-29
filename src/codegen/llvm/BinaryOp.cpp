#include "LLVMCodeGenerator.h"

using namespace codegen;

void LLVMCodeGenerator::visit(common::Add &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Left->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFAdd(Left, Right, "addtmp");
    } else {
        CurVal = Builder.CreateAdd(Left, Right, "addtmp");
    }
}

void LLVMCodeGenerator::visit(common::Sub &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFSub(Left, Right, "subtmp");
    } else {
        CurVal = Builder.CreateSub(Left, Right, "subtmp");
    }
}

void LLVMCodeGenerator::visit(common::Mul &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFMul(Left, Right, "multmp");
    } else {
        CurVal = Builder.CreateMul(Left, Right, "multmp");
    }
}

void LLVMCodeGenerator::visit(common::Div &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFDiv(Left, Right, "divtmp");
    } else {
        CurVal = Builder.CreateSDiv(Left, Right, "divtmp");
    }
}

void LLVMCodeGenerator::visit(common::Mod &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFRem(Left, Right, "modtmp");
    } else {
        CurVal = Builder.CreateSRem(Left, Right, "modtmp");
    }
}

void LLVMCodeGenerator::visit(common::And &Node) {
    assert(Node.Left->RetTy.Id == common::TypeId::BOOL &&
           Node.Right->RetTy.Id == common::TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateAnd(Left, Right, "andtmp");
}

void LLVMCodeGenerator::visit(common::Or &Node) {
    assert(Node.Left->RetTy.Id == common::TypeId::BOOL &&
           Node.Right->RetTy.Id == common::TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateOr(Left, Right, "ortmp");
}

void LLVMCodeGenerator::visit(common::Equal &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOEQ(Left, Right, "eqtmp");
    } else {
        CurVal = Builder.CreateICmpEQ(Left, Right, "eqtmp");
    }
}

void LLVMCodeGenerator::visit(common::NotEqual &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpONE(Left, Right, "neqtmp");
    } else {
        CurVal = Builder.CreateICmpNE(Left, Right, "neqtmp");
    }
}

void LLVMCodeGenerator::visit(common::Lesser &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOLT(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSLT(Left, Right, "lttmp");
    }
}

void LLVMCodeGenerator::visit(common::Greater &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOGT(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSGT(Left, Right, "lttmp");
    }
}

void LLVMCodeGenerator::visit(common::LesserEq &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOLE(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSLE(Left, Right, "lttmp");
    }
}

void LLVMCodeGenerator::visit(common::GreaterEq &Node) {
    assert((Node.Left->RetTy.Id == common::TypeId::FLOAT && Node.Right->RetTy.Id == common::TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == common::TypeId::INT && Node.Right->RetTy.Id == common::TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == common::TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOGE(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSGE(Left, Right, "lttmp");
    }
}
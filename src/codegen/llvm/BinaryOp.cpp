#include "LLVMCodeGen.h"

using namespace codegen;

void LLVMCodeGen::visit(common::Add &Node) {
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

void LLVMCodeGen::visit(common::Sub &Node) {
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

void LLVMCodeGen::visit(common::Mul &Node) {
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

void LLVMCodeGen::visit(common::Div &Node) {
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

void LLVMCodeGen::visit(common::Mod &Node) {
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

void LLVMCodeGen::visit(common::And &Node) {
    assert(Node.Left->RetTy.Id == common::TypeId::BOOL &&
           Node.Right->RetTy.Id == common::TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateAnd(Left, Right, "andtmp");
}

void LLVMCodeGen::visit(common::Or &Node) {
    assert(Node.Left->RetTy.Id == common::TypeId::BOOL &&
           Node.Right->RetTy.Id == common::TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateOr(Left, Right, "ortmp");
}

void LLVMCodeGen::visit(common::Equal &Node) {
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

void LLVMCodeGen::visit(common::NotEqual &Node) {
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

void LLVMCodeGen::visit(common::Lesser &Node) {
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

void LLVMCodeGen::visit(common::Greater &Node) {
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

void LLVMCodeGen::visit(common::LesserEq &Node) {
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

void LLVMCodeGen::visit(common::GreaterEq &Node) {
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
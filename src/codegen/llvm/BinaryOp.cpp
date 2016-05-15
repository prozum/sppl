#include "LLVMCodeGen.h"

using namespace codegen;
using namespace common;

void LLVMCodeGen::visit(Add &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Left->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFAdd(Left, Right, "addtmp");
    } else {
        CurVal = Builder.CreateAdd(Left, Right, "addtmp");
    }
}

void LLVMCodeGen::visit(Sub &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFSub(Left, Right, "subtmp");
    } else {
        CurVal = Builder.CreateSub(Left, Right, "subtmp");
    }
}

void LLVMCodeGen::visit(Mul &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFMul(Left, Right, "multmp");
    } else {
        CurVal = Builder.CreateMul(Left, Right, "multmp");
    }
}

void LLVMCodeGen::visit(Div &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFDiv(Left, Right, "divtmp");
    } else {
        CurVal = Builder.CreateSDiv(Left, Right, "divtmp");
    }
}

void LLVMCodeGen::visit(Mod &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFRem(Left, Right, "modtmp");
    } else {
        CurVal = Builder.CreateSRem(Left, Right, "modtmp");
    }
}

void LLVMCodeGen::visit(And &Node) {
    assert(Node.Left->RetTy.Id == TypeId::BOOL &&
           Node.Right->RetTy.Id == TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateAnd(Left, Right, "andtmp");
}

void LLVMCodeGen::visit(Or &Node) {
    assert(Node.Left->RetTy.Id == TypeId::BOOL &&
           Node.Right->RetTy.Id == TypeId::BOOL);

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    CurVal = Builder.CreateOr(Left, Right, "ortmp");
}

void LLVMCodeGen::visit(Equal &Node) {
    // TODO
    //assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
    //       (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOEQ(Left, Right, "eqtmp");
    } else {
        CurVal = Builder.CreateICmpEQ(Left, Right, "eqtmp");
    }
}

void LLVMCodeGen::visit(NotEqual &Node) {
    // TODO
    //assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
    //       (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpONE(Left, Right, "neqtmp");
    } else {
        CurVal = Builder.CreateICmpNE(Left, Right, "neqtmp");
    }
}

void LLVMCodeGen::visit(Lesser &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOLT(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSLT(Left, Right, "lttmp");
    }
}

void LLVMCodeGen::visit(Greater &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOGT(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSGT(Left, Right, "lttmp");
    }
}

void LLVMCodeGen::visit(LesserEq &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOLE(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSLE(Left, Right, "lttmp");
    }
}

void LLVMCodeGen::visit(GreaterEq &Node) {
    assert((Node.Left->RetTy.Id == TypeId::FLOAT && Node.Right->RetTy.Id == TypeId::FLOAT) ||
           (Node.Left->RetTy.Id == TypeId::INT && Node.Right->RetTy.Id == TypeId::INT));

    Node.Left->accept(*this);
    auto Left = CurVal;
    Node.Right->accept(*this);
    auto Right = CurVal;

    if (Node.Right->RetTy.Id == TypeId::FLOAT) {
        CurVal = Builder.CreateFCmpOGE(Left, Right, "lttmp");
    } else {
        CurVal = Builder.CreateICmpSGE(Left, Right, "lttmp");
    }
}

void LLVMCodeGen::visit(To &Node) {
    Node.Child->accept(*this);

    switch (Node.TypeCast.Id) {
        case TypeId::FLOAT:
            if (Node.Child->RetTy == TypeId::BOOL)
                CurVal = Builder.CreateUIToFP(CurVal, Double, "casttmp");
            else if (Node.Child->RetTy == TypeId::INT ||
                     Node.Child->RetTy == TypeId::CHAR)
                CurVal = Builder.CreateSIToFP(CurVal, Double, "casttmp");
            break;
        case TypeId::INT:
            if (Node.Child->RetTy == TypeId::FLOAT)
                CurVal = Builder.CreateFPToSI(CurVal, Int64, "casttmp");
            else
                CurVal = Builder.CreateTrunc(CurVal, Int64, "trunctmp");
            break;
        case TypeId::CHAR:
            if (Node.Child->RetTy == TypeId::FLOAT)
                CurVal = Builder.CreateFPToSI(CurVal, Int8, "casttmp");
            else
                CurVal = Builder.CreateTrunc(CurVal, Int8, "trunctmp");
            break;
        case TypeId::BOOL:
            if (Node.Child->RetTy == TypeId::FLOAT)
                CurVal = Builder.CreateFPToSI(CurVal, Int1, "casttmp");
            else
                CurVal = Builder.CreateTrunc(CurVal, Int1, "trunctmp");
            break;
        default:
            addError(Error::NotImplemented(Node.Loc));
    }
}
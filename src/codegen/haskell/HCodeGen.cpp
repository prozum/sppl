#include "HCodeGen.h"
#include <iostream>
#include <string>

using namespace std;
using namespace common;
using namespace codegen;

HCodeGen::HCodeGen(parser::Driver &Drv)
    : parser::CodeGenerator::CodeGenerator(Drv) {}

void HCodeGen::visit(Program &Node) {
    // os << "module PlaceHolder where" << endl;

    for (auto &Decl : Node.Decls) {
        Decl->accept(*this);
    }
}

void HCodeGen::visit(Function &Node) {
    CurFunc = &Node;

    if (Node.Id == "main")
        *Drv.Out << "main = putStrLn (show (";

    *Drv.Out << Node.Id << " :: ";

    if (Node.Signature.Subtypes.size() != 0) {
        for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
            *Drv.Out << Node.Signature.Subtypes[i].str() << " -> ";
        }

        *Drv.Out << Node.Signature.Subtypes.back().str();
    }

    *Drv.Out << endl;

    for (auto &Case : Node.Cases) {
        Case->accept(*this);
    }

    if (Node.Id == "main")
        *Drv.Out << "))";
    *Drv.Out << endl;
}

void HCodeGen::visit(Case &Node) {
    *Drv.Out << CurFunc->Id << " ";

    for (auto &Pattern : Node.Patterns) {
        Pattern->accept(*this);
        *Drv.Out << " ";
    }

    *Drv.Out << "= ";

    Node.Expr->accept(*this);

    *Drv.Out << endl;
}

void HCodeGen::visit(Or &Node) {
    *Drv.Out << "((||) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(And &Node) {
    *Drv.Out << "((&&) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Equal &Node) {
    *Drv.Out << "((==) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(NotEqual &Node) {
    *Drv.Out << "((/=) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Lesser &Node) {
    *Drv.Out << "((<) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(LesserEq &Node) {
    *Drv.Out << "((<=) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Greater &Node) {
    *Drv.Out << "((>) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(GreaterEq &Node) {
    *Drv.Out << "((>=) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Add &Node) {
    *Drv.Out << "((+) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Sub &Node) {
    *Drv.Out << "((-) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Mul &Node) {
    *Drv.Out << "((*) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Div &Node) {
    *Drv.Out << "((/) ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Mod &Node) {
    *Drv.Out << "(rem ";
    Node.Left->accept(*this);
    *Drv.Out << " ";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(ListAdd &Node) {
    Node.Left->accept(*this);
    *Drv.Out << ":";
    Node.Right->accept(*this);
}

void HCodeGen::visit(ParExpr &Node) {
    *Drv.Out << "(";
    Node.Child->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(Not &Node) {
    *Drv.Out << "not ";
    Node.Child->accept(*this);
}

void HCodeGen::visit(ListPattern &Node) {
    *Drv.Out << "[";

    if (Node.Patterns.size() != 0) {
        for (size_t i = 0; i < Node.Patterns.size() - 1; ++i) {
            Node.Patterns[i]->accept(*this);
            *Drv.Out << ",";
        }

        Node.Patterns.back()->accept(*this);
    }

    *Drv.Out << "]";
}

void HCodeGen::visit(TuplePattern &Node) {
    *Drv.Out << "(";

    if (Node.Patterns.size() != 0) {
        for (size_t i = 0; i < Node.Patterns.size() - 1; ++i) {
            Node.Patterns[i]->accept(*this);
            *Drv.Out << ",";
        }

        Node.Patterns.back()->accept(*this);
    }

    *Drv.Out << ")";
}

void HCodeGen::visit(ListSplit &Node) {
    *Drv.Out << "(";
    Node.Left->accept(*this);
    *Drv.Out << ":";
    Node.Right->accept(*this);
    *Drv.Out << ")";
}

void HCodeGen::visit(IntExpr &Node) { *Drv.Out << Node.Val; }

void HCodeGen::visit(FloatExpr &Node) { *Drv.Out << Node.Val; }

void HCodeGen::visit(BoolExpr &Node) { *Drv.Out << Node.Val; }

void HCodeGen::visit(CharExpr &Node) { *Drv.Out << Node.Val; }

void HCodeGen::visit(StringExpr &Node) { *Drv.Out << Node.Val; }

void HCodeGen::visit(ListExpr &Node) {
    *Drv.Out << "[";

    if (Node.Elements.size() != 0) {
        for (size_t i = 0; i < Node.Elements.size() - 1; ++i) {
            Node.Elements[i]->accept(*this);
            *Drv.Out << ",";
        }

        Node.Elements.back()->accept(*this);
    }

    *Drv.Out << "]";
}

void HCodeGen::visit(TupleExpr &Node) {
    *Drv.Out << "(";

    if (Node.Elements.size() != 0) {
        for (size_t i = 0; i < Node.Elements.size() - 1; ++i) {
            Node.Elements[i]->accept(*this);
            *Drv.Out << ",";
        }

        Node.Elements.back()->accept(*this);
    }

    *Drv.Out << ")";
}

void HCodeGen::visit(IdExpr &Node) { *Drv.Out << Node.Val; }

void HCodeGen::visit(CallExpr &Node) {
    *Drv.Out << "(";
    Node.Callee->accept(*this);

    *Drv.Out << " ";

    if (Node.Args.size() != 0) {
        for (size_t i = 0; i < Node.Args.size() - 1; ++i) {
            Node.Args[i]->accept(*this);
            *Drv.Out << " ";
        }

        Node.Args.back()->accept(*this);
    }
    *Drv.Out << ")";
}

#include "HCodeGenerator.h"
#include <iostream>
#include <string>

using namespace std;

namespace codegen {

    HCodeGenerator::HCodeGenerator(parser::Driver &Drv) : parser::CodeGenerator::CodeGenerator(Drv)
    {
    }

    void HCodeGenerator::visit(Program &Node)
    {
        //os << "module PlaceHolder where" << endl;

        for (auto &f : Node.Funcs) {
            if (f->Id != "main") {
                f->accept(*this);
            } else {
                *Drv.Out << "main = putStrLn (show (";
                f->Cases.front()->Expr->accept(*this);
                *Drv.Out << "))\n";
            }
        }
    }

    void HCodeGenerator::visit(Function &Node)
    {
        CurFunc = &Node;

        *Drv.Out << Node.Id << " :: ";

        if (Node.Signature.Subtypes.size() != 0) {
            for (size_t i = 0; i < Node.Signature.Subtypes.size() - 1; ++i) {
                *Drv.Out << Node.Signature.Subtypes[i].str() << " -> ";
            }

            *Drv.Out << Node.Signature.Subtypes.back().str();
        }

        *Drv.Out << endl;

        for (auto &c: Node.Cases) {
            c->accept(*this);
        }

        *Drv.Out << endl;
    }

    void HCodeGenerator::visit(Case &Node)
    {
        *Drv.Out << CurFunc->Id << " ";

        for (auto &p: Node.Patterns) {
            p->accept(*this);
            *Drv.Out << " ";
        }

        *Drv.Out << "= ";

        Node.Expr->accept(*this);

        *Drv.Out << endl;
    }

    void HCodeGenerator::visit(Or &Node)
    {
        *Drv.Out << "((||) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(And &Node)
    {
        *Drv.Out << "((&&) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Equal &Node)
    {
        *Drv.Out << "((==) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(NotEqual &Node)
    {
        *Drv.Out << "((/=) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Lesser &Node)
    {
        *Drv.Out << "((<) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(LesserEq &Node)
    {
        *Drv.Out << "((<=) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Greater &Node)
    {
        *Drv.Out << "((>) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(GreaterEq &Node)
    {
        *Drv.Out << "((>=) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Add &Node)
    {
        *Drv.Out << "((+) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Sub &Node)
    {
        *Drv.Out << "((-) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Mul &Node)
    {
        *Drv.Out << "((*) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Div &Node)
    {
        *Drv.Out << "((/) ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Mod &Node)
    {
        *Drv.Out << "(rem ";
        Node.Left->accept(*this);
        *Drv.Out << " ";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(ListAdd &Node)
    {
        Node.Left->accept(*this);
        *Drv.Out << ":";
        Node.Right->accept(*this);
    }

    void HCodeGenerator::visit(Par &Node)
    {
        *Drv.Out << "(";
        Node.Child->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Not &Node)
    {
        *Drv.Out << "not ";
        Node.Child->accept(*this);
    }

    void HCodeGenerator::visit(ListPattern &Node)
    {
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

    void HCodeGenerator::visit(TuplePattern &Node)
    {
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

    void HCodeGenerator::visit(ListSplit &Node)
    {
        *Drv.Out << "(";
        Node.Left->accept(*this);
        *Drv.Out << ":";
        Node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Int &Node)
    {
        *Drv.Out << Node.Val;
    }

    void HCodeGenerator::visit(Float &Node)
    {
        *Drv.Out << Node.Val;
    }

    void HCodeGenerator::visit(Bool &Node)
    {
        *Drv.Out << Node.Val;
    }

    void HCodeGenerator::visit(Char &Node)
    {
        *Drv.Out << Node.Val;
    }

    void HCodeGenerator::visit(String &Node)
    {
        *Drv.Out << Node.Val;
    }

    void HCodeGenerator::visit(List &Node)
    {
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

    void HCodeGenerator::visit(Tuple &Node)
    {
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

    void HCodeGenerator::visit(Id &Node)
    {
        *Drv.Out << Node.Val;
    }

    void HCodeGenerator::visit(Call &Node)
    {
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
}

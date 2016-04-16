#include "HCodeGenerator.h"
#include <iostream>
#include <string>

using namespace std;

namespace codegen {

    HCodeGenerator::HCodeGenerator(parser::Driver &Drv) : parser::CodeGenerator::CodeGenerator(Drv)
    {
    }

    void HCodeGenerator::visit(Program &node)
    {
        //os << "module PlaceHolder where" << endl;

        for (auto &f : node.Funcs) {
            if (f->Id != "main") {
                f->accept(*this);
            } else {
                *Drv.Out << "main = putStrLn (show (";
                f->Cases.front()->Expr->accept(*this);
                *Drv.Out << "))\n";
            }
        }
    }

    void HCodeGenerator::visit(Function &node)
    {
        curr_fun = &node;

        *Drv.Out << node.Id << " :: ";

        if (node.Signature.Subtypes.size() != 0) {
            for (size_t i = 0; i < node.Signature.Subtypes.size() - 1; ++i) {
                *Drv.Out << node.Signature.Subtypes[i].str() << " -> ";
            }

            *Drv.Out << node.Signature.Subtypes.back().str();
        }

        *Drv.Out << endl;

        for (auto &c: node.Cases) {
            c->accept(*this);
        }

        *Drv.Out << endl;
    }

    void HCodeGenerator::visit(Case &node)
    {
        *Drv.Out << curr_fun->Id << " ";

        for (auto &p: node.Patterns) {
            p->accept(*this);
            *Drv.Out << " ";
        }

        *Drv.Out << "= ";

        node.Expr->accept(*this);

        *Drv.Out << endl;
    }

    void HCodeGenerator::visit(Or &node)
    {
        *Drv.Out << "((||) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(And &node)
    {
        *Drv.Out << "((&&) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Equal &node)
    {
        *Drv.Out << "((==) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(NotEqual &node)
    {
        *Drv.Out << "((/=) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Lesser &node)
    {
        *Drv.Out << "((<) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(LesserEq &node)
    {
        *Drv.Out << "((<=) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Greater &node)
    {
        *Drv.Out << "((>) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(GreaterEq &node)
    {
        *Drv.Out << "((>=) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Add &node)
    {
        *Drv.Out << "((+) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Sub &node)
    {
        *Drv.Out << "((-) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Mul &node)
    {
        *Drv.Out << "((*) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Div &node)
    {
        *Drv.Out << "((/) ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Mod &node)
    {
        *Drv.Out << "(rem ";
        node.Left->accept(*this);
        *Drv.Out << " ";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(ListAdd &node)
    {
        node.Left->accept(*this);
        *Drv.Out << ":";
        node.Right->accept(*this);
    }

    void HCodeGenerator::visit(Par &node)
    {
        *Drv.Out << "(";
        node.Child->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Not &node)
    {
        *Drv.Out << "not ";
        node.Child->accept(*this);
    }

    void HCodeGenerator::visit(ListPattern &node)
    {
        *Drv.Out << "[";

        if (node.Patterns.size() != 0) {
            for (size_t i = 0; i < node.Patterns.size() - 1; ++i) {
                node.Patterns[i]->accept(*this);
                *Drv.Out << ",";
            }

            node.Patterns.back()->accept(*this);
        }

        *Drv.Out << "]";
    }

    void HCodeGenerator::visit(TuplePattern &node)
    {
        *Drv.Out << "(";

        if (node.Patterns.size() != 0) {
            for (size_t i = 0; i < node.Patterns.size() - 1; ++i) {
                node.Patterns[i]->accept(*this);
                *Drv.Out << ",";
            }

            node.Patterns.back()->accept(*this);
        }

        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(ListSplit &node)
    {
        *Drv.Out << "(";
        node.Left->accept(*this);
        *Drv.Out << ":";
        node.Right->accept(*this);
        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Int &node)
    {
        *Drv.Out << node.Val;
    }

    void HCodeGenerator::visit(Float &node)
    {
        *Drv.Out << node.Val;
    }

    void HCodeGenerator::visit(Bool &node)
    {
        *Drv.Out << node.Val;
    }

    void HCodeGenerator::visit(Char &node)
    {
        *Drv.Out << node.Val;
    }

    void HCodeGenerator::visit(String &node)
    {
        *Drv.Out << node.Val;
    }

    void HCodeGenerator::visit(List &node)
    {
        *Drv.Out << "[";

        if (node.Elements.size() != 0) {
            for (size_t i = 0; i < node.Elements.size() - 1; ++i) {
                node.Elements[i]->accept(*this);
                *Drv.Out << ",";
            }

            node.Elements.back()->accept(*this);
        }

        *Drv.Out << "]";
    }

    void HCodeGenerator::visit(Tuple &node)
    {
        *Drv.Out << "(";

        if (node.Elements.size() != 0) {
            for (size_t i = 0; i < node.Elements.size() - 1; ++i) {
                node.Elements[i]->accept(*this);
                *Drv.Out << ",";
            }

            node.Elements.back()->accept(*this);
        }

        *Drv.Out << ")";
    }

    void HCodeGenerator::visit(Id &node)
    {
        *Drv.Out << node.Val;
    }

    void HCodeGenerator::visit(Call &node)
    {
        *Drv.Out << "(";
        node.Callee->accept(*this);

        *Drv.Out << " ";

        if (node.Args.size() != 0) {
            for (size_t i = 0; i < node.Args.size() - 1; ++i) {
                node.Args[i]->accept(*this);
                *Drv.Out << " ";
            }

            node.Args.back()->accept(*this);
        }
        *Drv.Out << ")";
    }
}

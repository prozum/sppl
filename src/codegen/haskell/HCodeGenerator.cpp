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

        for (auto &f : node.funcs) {
            if (f->id != "main") {
                f->accept(*this);
            } else {
                *driver.out << "main = putStrLn (show (";
                f->cases.front()->expr->accept(*this);
                *driver.out << "))\n";
            }
        }
    }

    void HCodeGenerator::visit(Function &node)
    {
        curr_fun = &node;

        *driver.out << node.id << " :: ";

        if (node.signature.subtypes.size() != 0) {
            for (size_t i = 0; i < node.signature.subtypes.size() - 1; ++i) {
                *driver.out << node.signature.subtypes[i].str() << " -> ";
            }

            *driver.out << node.signature.subtypes.back().str();
        }

        *driver.out << endl;

        for (auto &c: node.cases) {
            c->accept(*this);
        }

        *driver.out << endl;
    }

    void HCodeGenerator::visit(Case &node)
    {
        *driver.out << curr_fun->id << " ";

        for (auto &p: node.patterns) {
            p->accept(*this);
            *driver.out << " ";
        }

        *driver.out << "= ";

        node.expr->accept(*this);

        *driver.out << endl;
    }

    void HCodeGenerator::visit(Or &node)
    {
        *driver.out << "((||) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(And &node)
    {
        *driver.out << "((&&) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Equal &node)
    {
        *driver.out << "((==) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(NotEqual &node)
    {
        *driver.out << "((/=) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Lesser &node)
    {
        *driver.out << "((<) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(LesserEq &node)
    {
        *driver.out << "((<=) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Greater &node)
    {
        *driver.out << "((>) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(GreaterEq &node)
    {
        *driver.out << "((>=) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Add &node)
    {
        *driver.out << "((+) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Sub &node)
    {
        *driver.out << "((-) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Mul &node)
    {
        *driver.out << "((*) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Div &node)
    {
        *driver.out << "((/) ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Mod &node)
    {
        *driver.out << "(rem ";
        node.left->accept(*this);
        *driver.out << " ";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(ListAdd &node)
    {
        node.left->accept(*this);
        *driver.out << ":";
        node.right->accept(*this);
    }

    void HCodeGenerator::visit(Par &node)
    {
        *driver.out << "(";
        node.child->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Not &node)
    {
        *driver.out << "not ";
        node.child->accept(*this);
    }

    void HCodeGenerator::visit(ListPattern &node)
    {
        *driver.out << "[";

        if (node.patterns.size() != 0) {
            for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
                node.patterns[i]->accept(*this);
                *driver.out << ",";
            }

            node.patterns.back()->accept(*this);
        }

        *driver.out << "]";
    }

    void HCodeGenerator::visit(TuplePattern &node)
    {
        *driver.out << "(";

        if (node.patterns.size() != 0) {
            for (size_t i = 0; i < node.patterns.size() - 1; ++i) {
                node.patterns[i]->accept(*this);
                *driver.out << ",";
            }

            node.patterns.back()->accept(*this);
        }

        *driver.out << ")";
    }

    void HCodeGenerator::visit(ListSplit &node)
    {
        *driver.out << "(";
        node.left->accept(*this);
        *driver.out << ":";
        node.right->accept(*this);
        *driver.out << ")";
    }

    void HCodeGenerator::visit(Int &node)
    {
        *driver.out << node.value;
    }

    void HCodeGenerator::visit(Float &node)
    {
        *driver.out << node.value;
    }

    void HCodeGenerator::visit(Bool &node)
    {
        *driver.out << node.value;
    }

    void HCodeGenerator::visit(Char &node)
    {
        *driver.out << node.value;
    }

    void HCodeGenerator::visit(String &node)
    {
        *driver.out << node.value;
    }

    void HCodeGenerator::visit(List &node)
    {
        *driver.out << "[";

        if (node.exprs.size() != 0) {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                node.exprs[i]->accept(*this);
                *driver.out << ",";
            }

            node.exprs.back()->accept(*this);
        }

        *driver.out << "]";
    }

    void HCodeGenerator::visit(Tuple &node)
    {
        *driver.out << "(";

        if (node.exprs.size() != 0) {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                node.exprs[i]->accept(*this);
                *driver.out << ",";
            }

            node.exprs.back()->accept(*this);
        }

        *driver.out << ")";
    }

    void HCodeGenerator::visit(Id &node)
    {
        *driver.out << node.id;
    }

    void HCodeGenerator::visit(Call &node)
    {
        *driver.out << "(";
        node.callee->accept(*this);

        *driver.out << " ";

        if (node.exprs.size() != 0) {
            for (size_t i = 0; i < node.exprs.size() - 1; ++i) {
                node.exprs[i]->accept(*this);
                *driver.out << " ";
            }

            node.exprs.back()->accept(*this);
        }
        *driver.out << ")";
    }
}

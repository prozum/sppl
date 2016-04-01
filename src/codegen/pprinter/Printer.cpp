#include "Printer.h"
#include <iostream>

using namespace std;

namespace codegen {
    Printer::Printer(ostream &os) 
            : CodeGenerator::CodeGenerator(os)
    {
    }
    
    void Printer::visit(Program &node) {
        for (auto& func: node.funcs) {
            func->accept(*this);
        }
    }

    void Printer::visit(Function &node) {
        output << "def " << node.id << " : ";

        for (auto& p: node.types) {
            p->accept(*this);
            if (p != node.types.back())
                output << " -> ";
        }

        output << endl;

        for (auto& _case : node.cases) {
            _case->accept(*this);
            output << endl;
        }

        output << endl;
    }

    void Printer::visit(Case &node) {
        output << "\t| ";

        for (auto & p: node.patterns) {
            p->accept(*this);
            if (p != node.patterns.back())
                output << " ";
        }

        output << " = ";

        node.expr->accept(*this);
    }

    void Printer::visit(Or &node) {
        node.left->accept(*this);
        output << " || ";
        node.right->accept(*this);
    }

    void Printer::visit(And &node) {
        node.left->accept(*this);
        output << " && ";
        node.right->accept(*this);
    }

    void Printer::visit(Equal &node) {
        node.left->accept(*this);
        output << " == ";
        node.right->accept(*this);
    }

    void Printer::visit(NotEqual &node) {
        node.left->accept(*this);
        output << " != ";
        node.right->accept(*this);
    }

    void Printer::visit(Lesser &node) {
        node.left->accept(*this);
        output << " < ";
        node.right->accept(*this);
    }

    void Printer::visit(Greater &node) {
        node.left->accept(*this);
        output << " > ";
        node.right->accept(*this);
    }

    void Printer::visit(LesserEq &node) {
        node.left->accept(*this);
        output << " <= ";
        node.right->accept(*this);
    }

    void Printer::visit(GreaterEq &node) {
        node.left->accept(*this);
        output << " >= ";
        node.right->accept(*this);
    }

    void Printer::visit(Add &node) {
        node.left->accept(*this);
        output << " + ";
        node.right->accept(*this);
    }

    void Printer::visit(Sub &node) {
        node.left->accept(*this);
        output << " - ";
        node.right->accept(*this);
    }

    void Printer::visit(Mul &node) {
        node.left->accept(*this);
        output << " * ";
        node.right->accept(*this);
    }

    void Printer::visit(Div &node) {
        node.left->accept(*this);
        output << " / ";
        node.right->accept(*this);
    }

    void Printer::visit(Mod &node) {
        node.left->accept(*this);
        output << " % ";
        node.right->accept(*this);
    }

    void Printer::visit(ListAdd &node) {
        node.left->accept(*this);
        output << " : ";
        node.right->accept(*this);
    }

    void Printer::visit(Par &node) {
        output << "(";
        node.child->accept(*this);
        output << ")";
    }

    void Printer::visit(Not &node) {
        output << "!";
        node.child->accept(*this);
    }

    void Printer::visit(Int &node) {
        output << node.value;
    }

    void Printer::visit(Float &node) {
        output << node.value;

        if (node.value == (int)node.value)
            output << ".0";
    }

    void Printer::visit(Bool &node) {
        output << (node.value ? "True" : "False");
    }

    void Printer::visit(Char &node) {
        output << "'";
        output << node.value;
        output << "'";
    }

    void Printer::visit(String &node) {
        output << "\"";
        output << node.value;
        output << "\"";
    }

    void Printer::visit(List &node) {
        output << "[";

        for (auto & p: node.exprs) {
            p->accept(*this);
            if (p != node.exprs.back())
                output << ", ";
        }

        output << "]";
    }

    void Printer::visit(Tuple &node) {
        output << "(";

        for (auto & p: node.exprs) {
            p->accept(*this);
            if (p != node.exprs.back())
                output << ", ";
        }

        output << ")";
    }

    void Printer::visit(Id &node) {
        output << node.id;
    }

    void Printer::visit(Call &node) {
        node.callee->accept(*this);
        output << "(";

        for (auto & p: node.exprs) {
            p->accept(*this);
            if (p != node.exprs.back())
                output << ", ";
        }

        output << ")";
    }

    void Printer::visit(Type &node) {
        switch (node.type) {
            case Types::INT:
                output << "Int";
                break;
            case Types::FLOAT:
                output << "Float";
                break;
            case Types::BOOL:
                output << "Bool";
                break;
            case Types::CHAR:
                output << "Char";
                break;
            case Types::STRING:
                output << "String";
                break;
            case Types::LIST:
                output << "[";
                node.types[0]->accept(*this);
                output << "]";
                break;
            case Types::TUPLE:
                output << "(";
                print_collection(node, ", ");
                output << ")";
                break;
            case Types::SIGNATURE:
                output << "(";
                print_collection(node, " -> ");
                output << ")";
                break;
            default:
                break;
        }
    }

    void Printer::print_collection(Type &node, const std::string split) {
        for (auto & p: node.types) {
            p->accept(*this);
            if (p != node.types.back())
                output << split;
        }
    }

    void Printer::visit(ListPattern &node) {
        output << "[";

        for (auto & p: node.patterns) {
            p->accept(*this);
            if (p != node.patterns.back())
                output << ", ";
        }

        output << "]";
    }

    void Printer::visit(TuplePattern &node) {
        output << "(";

        for (auto & p: node.patterns) {
            p->accept(*this);
            if (p != node.patterns.back())
                output << ", ";
        }

        output << ")";
    }

    void Printer::visit(ListSplit &node) {
        output << "(";
        node.left->accept(*this);
        output << " : ";
        node.right->accept(*this);
        output << ")";
    }
}

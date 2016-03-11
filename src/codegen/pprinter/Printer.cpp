#include "Printer.h"
#include <iostream>

using namespace std;

namespace codegen {
    Printer::Printer(ostream &os) 
            : CodeGenerator::CodeGenerator(os)
    {
    }
    
    void Printer::visit(Program &node) {
        cout << "Program " << endl;

        for (auto& func: node.funcs) {
            func->accept(*this);
        }
    }

    void Printer::visit(Function &node) {
        cout << "Function " << node.id << endl;

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

        cout << "Case" << endl;

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

        cout << "Or" << endl;

        node.left->accept(*this);
        output << " || ";
        node.right->accept(*this);
    }

    void Printer::visit(And &node) {

        cout << "And" << endl;

        node.left->accept(*this);
        output << " && ";
        node.right->accept(*this);
    }

    void Printer::visit(Equal &node) {

        cout << "Equal" << endl;

        node.left->accept(*this);
        output << " == ";
        node.right->accept(*this);
    }

    void Printer::visit(NotEqual &node) {

        cout << "Not Equal" << endl;

        node.left->accept(*this);
        output << " != ";
        node.right->accept(*this);
    }

    void Printer::visit(Lesser &node) {

        cout << "Lesser" << endl;

        node.left->accept(*this);
        output << " < ";
        node.right->accept(*this);
    }

    void Printer::visit(Greater &node) {

        cout << "Greater" << endl;

        node.left->accept(*this);
        output << " > ";
        node.right->accept(*this);
    }

    void Printer::visit(LesserEq &node) {

        cout << "LesserEq" << endl;

        node.left->accept(*this);
        output << " <= ";
        node.right->accept(*this);
    }

    void Printer::visit(GreaterEq &node) {

        cout << "GreaterEq" << endl;

        node.left->accept(*this);
        output << " >= ";
        node.right->accept(*this);
    }

    void Printer::visit(Add &node) {

        cout << "Add" << endl;

        node.left->accept(*this);
        output << " + ";
        node.right->accept(*this);
    }

    void Printer::visit(Sub &node) {

        cout << "Sub" << endl;

        node.left->accept(*this);
        output << " - ";
        node.right->accept(*this);
    }

    void Printer::visit(Mul &node) {

        cout << "Mul" << endl;

        node.left->accept(*this);
        output << " * ";
        node.right->accept(*this);
    }

    void Printer::visit(Div &node) {

        cout << "Div" << endl;

        node.left->accept(*this);
        output << " / ";
        node.right->accept(*this);
    }

    void Printer::visit(Mod &node) {

        cout << "Mod" << endl;

        node.left->accept(*this);
        output << " % ";
        node.right->accept(*this);
    }

    void Printer::visit(ListAdd &node) {

        cout << "ListAdd" << endl;

        node.left->accept(*this);
        output << " : ";
        node.right->accept(*this);
    }

    void Printer::visit(Par &node) {

        cout << "Par" << endl;

        output << "(";
        node.child->accept(*this);
        output << ")";
    }

    void Printer::visit(Not &node) {

        cout << "Not" << endl;

        output << "!";
        node.child->accept(*this);
    }

    void Printer::visit(Int &node) {

        cout << "Int" << endl;

        output << node.value;
    }

    void Printer::visit(Float &node) {

        cout << "Float" << endl;

        output << node.value;
    }

    void Printer::visit(Bool &node) {

        cout << "Bool" << endl;

        output << (node.value ? "True" : "False");
    }

    void Printer::visit(Char &node) {

        cout << "Char" << endl;

        output << "'";
        output << node.value;
        output << "'";
    }

    void Printer::visit(String &node) {

        cout << "String" << endl;

        output << "\"";
        output << node.value;
        output << "\"";
    }

    void Printer::visit(List &node) {

        cout << "List" << endl;

        output << "[";

        for (auto & p: node.exprs) {
            p->accept(*this);
            if (p != node.exprs.back())
                output << ", ";
        }

        output << "]";
    }

    void Printer::visit(Tuple &node) {

        cout << "Tuple" << endl;

        output << "(";

        for (auto & p: node.exprs) {
            p->accept(*this);
            if (p != node.exprs.back())
                output << ", ";
        }

        output << ")";
    }

    void Printer::visit(Id &node) {

        cout << "Id" << endl;

        output << node.id;
    }

    void Printer::visit(Call &node) {

        cout << "Call " << node.exprs.size() << endl;

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

        cout << "Type" << endl;

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

        cout << "ListPattern" << endl;

        output << "[";

        for (auto & p: node.patterns) {
            p->accept(*this);
            if (p != node.patterns.back())
                output << ", ";
        }

        output << "]";
    }

    void Printer::visit(TuplePattern &node) {

        cout << "TuplePattern" << endl;

        output << "(";

        for (auto & p: node.patterns) {
            p->accept(*this);
            if (p != node.patterns.back())
                output << ", ";
        }

        output << ")";
    }

    void Printer::visit(ListSplit &node) {

        cout << "ListSplit" << endl;

        output << "(";
        node.left->accept(*this);
        output << " : ";
        node.right->accept(*this);
        output << ")";
    }
}

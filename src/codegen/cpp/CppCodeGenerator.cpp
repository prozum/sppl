#include "CppCodeGenerator.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace common;
using namespace std;

namespace codegen
{
    CCodeGenerator::CCodeGenerator(std::ostream &out) : CodeGenerator::CodeGenerator(out) {
    }

    void CCodeGenerator::visit(Program &node) {
        for (auto f : node.funcs) {
            f->accept(*this);
        }
    }

    void CCodeGenerator::visit(Function &node) {
        is_return = true;
        node.types.back()->accept(*this);
        is_return = false;

        output << " u" << node.id << "( ";

        for (auto type : node.types) {
            type->accept(*this);
            arg_count++;

            if (type != node.types.back())
                output << ", ";
        }

        arg_count = 0;

        output << ") {" << endl;

        for (auto c : node.cases) {
            c->accept(*this);
            real_ids.clear();
        }

        output << "}" << endl;
        arg_names.clear();
    }

    void CCodeGenerator::visit(Case &node) {
        output << "if (";

        context = PATTERN;
        for (auto pattern : node.patterns) {
            output << " g" << arg_count << " == ";
            pattern->accept(*this);

            if (pattern != node.patterns.back())
                output << " && ";

            arg_count++;
        }

        arg_count = 0;
        context = EXPR;

        output << ") {" << endl;
        output << "return ";
        node.expr->accept(*this);
        output << endl << "}" << endl;
    }

    void CCodeGenerator::visit(Or &node) {
        output << "(";
        node.left->accept(*this);
        output << "||";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(And &node) {
        output << "(";
        node.left->accept(*this);
        output << "&&";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Equal &node) {
        output << "(";
        node.left->accept(*this);
        output << "==";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(NotEqual &node) {
        output << "(";
        node.left->accept(*this);
        output << "!=";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Lesser &node) {
        output << "(";
        node.left->accept(*this);
        output << "<";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(LesserEq &node) {
        output << "(";
        node.left->accept(*this);
        output << "<=";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Greater &node) {
        output << "(";
        node.left->accept(*this);
        output << ">";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(GreaterEq &node) {
        output << "(";
        node.left->accept(*this);
        output << ">=";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Add &node) {
        output << "(";
        node.left->accept(*this);
        output << "+";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Sub &node) {
        output << "(";
        node.left->accept(*this);
        output << "-";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Mul &node) {
        output << "(";
        node.left->accept(*this);
        output << "*";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Div &node) {
        output << "(";
        node.left->accept(*this);
        output << "/";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Mod &node) {
        output << "(";
        node.left->accept(*this);
        output << "%";
        node.right->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(ListAdd &node) {

    }

    void CCodeGenerator::visit(Par &node) {
        output << "(";
        node.child->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(Not &node) {
        output << "(";
        output << "!";
        node.child->accept(*this);
        output << ")";
    }

    void CCodeGenerator::visit(ListPattern &node) {

    }

    void CCodeGenerator::visit(TuplePattern &node) {

    }

    void CCodeGenerator::visit(ListSplit &node) {

    }

    void CCodeGenerator::visit(Int &node) {
        output << node.value;
    }

    void CCodeGenerator::visit(Float &node) {
        output << node.value;
    }

    void CCodeGenerator::visit(Bool &node) {
        output << node.value;
    }

    void CCodeGenerator::visit(Char &node) {
        output << node.value;
    }

    void CCodeGenerator::visit(String &node) {
        output << node.value;
    }

    void CCodeGenerator::visit(List &node) {

    }

    void CCodeGenerator::visit(Tuple &node) {

    }

    void CCodeGenerator::visit(Id &node) {
        switch (context) {
            case PATTERN:
                real_ids.insert({node.id, arg_names[arg_count]});
                output << arg_names[arg_count];
                break;
            case EXPR:
                output << real_ids[node.id];
                break;
            default:
                throw "WOW! THIS SHOULD NEVER HAPPEN";
        }
    }

    void CCodeGenerator::visit(Call &node) {
        output << "(";
        node.callee->accept(*this);

        output << " ";

        for (auto expr : node.exprs) {
            expr->accept(*this);

            if (expr != node.exprs.back())
                output << ", ";
        }

        output << ")";
    }

    void CCodeGenerator::visit(Type &node) {
        stringstream arg_name;

        switch (node.type) {
            case FLOAT:
                output << "double";
            case CHAR:
                output << "char";
            case INT:
                output << "int";
            case BOOL:
                output << "int";

            case STRING:
            case LIST:
            case TUPLE:
            case SIGNATURE:
            default:
                break;
        }

        if (!is_return) {
            arg_name << " g" << arg_count;
            output << arg_name.str();
            arg_names.push_back(arg_name.str());
        }
    }
}
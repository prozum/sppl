#include "Node.h"

#include "Visitor.h"

using namespace std;

namespace common {

    void Program::accept(Visitor &v) { v.visit(*this); }
    void Function::accept(Visitor &v) { v.visit(*this); }
    void Case::accept(Visitor &v) { v.visit(*this); }

    void List::accept(Visitor &v) { v.visit(*this); }
    void Tuple::accept(Visitor &v) { v.visit(*this); }
    void Call::accept(Visitor &v) { v.visit(*this); }

    Node::Node(Location loc) :
            Ty(TypeId::UNKNOWN), Loc(loc) { }

    Node::Node(Type type, Location loc) :
            Ty(type), Loc(loc) { }

    Expression::Expression(Location loc) :
            Node(loc) { }

    Expression::Expression(Type type, Location loc) :
            Node(type, loc) { }


    Program::Program(vector<unique_ptr<Function>> funcs,
                     Location loc) :
            Node(loc),
            Funcs(move(funcs)) { }

    Program::Program(unique_ptr<Expression> expr,
                     Location loc) :
                     Node(loc) {

        Funcs.push_back(make_unique<Function>(move(expr)));
    }

    Function::Function(unique_ptr<Expression> expr) :
            Node(expr->Loc),
            Id(ANON_FUNC_NAME),
            Signature(Type(TypeId::UNKNOWN)),
            Anon(true) {
        Cases.push_back(make_unique<Case>(move(expr), vector<unique_ptr<Pattern>>(), expr->Loc));
    }

    Function::Function(string id,
                       Type sign,
                       Location loc) :
            Node(sign.Subtypes.front(), loc),
            Id(id),
            Signature(sign) { }

    Case::Case(unique_ptr<Expression> expr,
               vector<unique_ptr<Pattern>> patterns,
               Location loc) :
            Node(loc),
            Expr(move(expr)),
            Patterns(move(patterns)) { }

    Call::Call(unique_ptr<Expression> callee,
               vector<unique_ptr<Expression>> exprs,
               Location loc) :
            Expression(loc),
            Callee(move(callee)),
            Args(move(exprs))
    {
    }

    string Program::str() {
        string str;
        for (auto &func : Funcs) {
            str += func->str();
        }

        return str;
    }

    string Function::str() {
        string str("def " + Id + " : ");

        for (auto &type : Signature.Subtypes) {
            str += type.str();
            if (type != Signature.Subtypes.back())
                str += " -> ";
        }

        str += '\n';

        for (auto &cse : Cases) {
            str += cse->str() + "\n";
        }

        return str + "\n";
    }

    string Case::str() {
        string str("\t| ");

        for (auto &pattern : Patterns) {
            str += pattern->str();
            if (pattern != Patterns.back())
                str += ' ';
        }

        return str + " = " + Expr->str();
    }

    string Call::str() {
        string str(Callee->str() + "(");

        for (auto &expr : Args) {
            str += expr->str();
            if (expr != Args.back())
                str += ", ";
        }

        return str + ")";
    }


}

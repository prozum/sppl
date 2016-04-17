#include "Node.h"

#include "Visitor.h"

using namespace std;

namespace common {

    void Program::accept(Visitor &V) { V.visit(*this); }
    void Function::accept(Visitor &V) { V.visit(*this); }
    void Case::accept(Visitor &V) { V.visit(*this); }

    void List::accept(Visitor &V) { V.visit(*this); }
    void Tuple::accept(Visitor &V) { V.visit(*this); }
    void Call::accept(Visitor &V) { V.visit(*this); }

    Node::Node(Location Loc) :
            Ty(TypeId::UNKNOWN), Loc(Loc) { }

    Node::Node(Type Ty, Location Loc) :
            Ty(Ty), Loc(Loc) { }

    Expression::Expression(Location Loc) :
            Node(Loc) { }

    Expression::Expression(Type Ty, Location Loc) :
            Node(Ty, Loc) { }


    Program::Program(vector<unique_ptr<Function>> Funcs,
                     Location Loc) :
            Node(Loc),
            Funcs(move(Funcs)) { }

    Program::Program(unique_ptr<Expression> AnonFunc,
                     Location Loc) :
                     Node(Loc) {

        Funcs.push_back(make_unique<Function>(move(AnonFunc)));
    }

    Function::Function(unique_ptr<Expression> AnonFunc) :
            Node(AnonFunc->Loc),
            Id(ANON_FUNC_NAME),
            Signature(Type(TypeId::UNKNOWN)),
            Anon(true) {
        Cases.push_back(make_unique<Case>(move(AnonFunc), vector<unique_ptr<Pattern>>(), AnonFunc->Loc));
    }

    Function::Function(string Id,
                       Type Ty,
                       Location Loc) :
            Node(Ty.Subtypes.front(), Loc),
            Id(Id),
            Signature(Ty) { }

    Case::Case(unique_ptr<Expression> Expr,
               vector<unique_ptr<Pattern>> Patterns,
               Location Loc) :
            Node(Loc),
            Expr(move(Expr)),
            Patterns(move(Patterns)) { }

    Call::Call(unique_ptr<Expression> Callee,
               vector<unique_ptr<Expression>> Args,
               Location Loc) :
            Expression(Loc),
            Callee(move(Callee)),
            Args(move(Args))
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

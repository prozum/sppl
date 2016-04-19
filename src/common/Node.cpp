#include "Node.h"
#include "Expression.h"
#include "Visitor.h"

using namespace std;

namespace common {

    void Program::accept(Visitor &V) { V.visit(*this); }
    void Function::accept(Visitor &V) { V.visit(*this); }
    void Case::accept(Visitor &V) { V.visit(*this); }
    void LambdaArg::accept(Visitor &V) { V.visit(*this); }
    void AlgebraicDT::accept(Visitor &V) { V.visit(*this); }
    void Product::accept(Visitor &V) { V.visit(*this); }

    Node::Node(Location Loc) :
            Loc(Loc) { }

    Program::Program(vector<unique_ptr<Declaration>> Decls,
                     Location Loc) :
            Node(Loc),
            Decls(move(Decls)) { }

    Program::Program(unique_ptr<Expression> AnonFunc,
                     Location Loc) :
                     Node(Loc) {

        Decls.push_back(make_unique<Function>(move(AnonFunc)));
    }

    Declaration::Declaration(Location Loc) :
            Node(Loc) { }

    Declaration::Declaration(Type Ty, Location Loc) :
            Node(Loc) { }

    Function::Function(unique_ptr<Expression> AnonFunc) :
            Declaration(AnonFunc->Loc),
            Id(ANON_FUNC_NAME),
            Signature(Type(TypeId::UNKNOWN)),
            Anon(true) {
        Cases.push_back(make_unique<Case>(move(AnonFunc), nullptr, vector<unique_ptr<Pattern>>(), AnonFunc->Loc));
    }

    Function::Function(string Id,
                       Type Ty,
                       Location Loc) :
            Declaration(Ty.Subtypes.front(), Loc),
            Id(Id),
            Signature(Ty) { }

    Case::Case(unique_ptr<Expression> Expr,
               unique_ptr<Expression> When,
               vector<unique_ptr<Pattern>> Patterns,
               Location Loc) :
            Node(Loc),
            Expr(move(Expr)),
            When(move(When)),
            Patterns(move(Patterns)) { }

    LambdaArg::LambdaArg(string Id,
                         Location Loc) :
            Node(Loc),
            Id(Id) { }

    AlgebraicDT::AlgebraicDT(string Name, vector<Type> TypeConstructor, vector<unique_ptr<Product>> Sum,
                             Location Loc) :
            Declaration(Loc),
            Name(Name),
            TypeConstructor(TypeConstructor),
            Sum(move(Sum)) { }

    Product::Product(string Constructor,
                     vector<Type> Values,
                     Location Loc) :
            Node(Loc),
            Constructor(Constructor),
            Values(Values) { }

    string Program::str() {
        string Str;
        for (auto &Func : Decls) {
            Str += Func->str();
        }

        return Str;
    }

    string Function::str() {
        string Str("def " + Id + " : ");

        for (size_t I = 0; I < Signature.Subtypes.size(); I++) {
            Str += Signature.Subtypes[I].str();

            if (I != Signature.Subtypes.size() - 1)
                Str += " -> ";
        }

        Str += "\n";

        for (auto &Case : Cases) {
            Str += Case->str() + "\n";
        }

        return Str + "\n\n";
    }

    string Case::str() {
        if (When)
            return "\t| " + strJoin(Patterns, " ") + " when " + When->str() + " = " + Expr->str();
        else
            return "\t| " + strJoin(Patterns, " ") + " = " + Expr->str();
    }

    string LambdaArg::str() {
        return Id;
    }

    string AlgebraicDT::str() {
        string Str("type " + Name);

        for (auto Type : TypeConstructor) {
            Str += " " + Type.str();
        }

        return Str + " = " + strJoin(Sum, " | ") + "\n\n";
    }

    string Product::str() {
        string Str(Constructor);

        for (auto Value : Values) {
            Str += " " + Value.str();
        }

        return Str;
    }

    template<class T>
    string strJoin(T &List, const std::string JoinStr) {
        string Str;
        for (size_t i = 0; i < List.size(); ++i) {
            Str += List[i]->str();
            if (i + 1 != List.size())
                Str += JoinStr;
        }

        return Str;
    }
}

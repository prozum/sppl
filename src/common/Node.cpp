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

    unique_ptr<Node> Program::clone() {
        auto Res = new Program(vector<unique_ptr<Declaration>>(), Loc);

        for (auto &Decl: Decls) {
            auto Clone = Decl->clone().release();
            Res->Decls.push_back(unique_ptr<Declaration>((Declaration*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> Function::clone() {
        auto Res = new Function(Id, Signature, Loc);

        for (auto &Cse: Cases) {
            auto Clone = Cse->clone().release();
            Res->Cases.push_back(unique_ptr<Case>((Case*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> Case::clone() {
        Case* Res;
        auto NewExpr = Expr->clone().release();
        if (When) {
            auto NewWhen = When->clone().release();
            Res = new Case(unique_ptr<Expression>((Expression*)NewExpr), unique_ptr<Expression>((Expression*)NewWhen), vector<unique_ptr<Pattern>>(), Loc);
        } else {
            Res = new Case(unique_ptr<Expression>((Expression*)NewExpr), nullptr, vector<unique_ptr<Pattern>>(), Loc);
        }

        for (auto &Pat: Patterns) {
            auto Clone = Pat->clone().release();
            Res->Patterns.push_back(unique_ptr<Pattern>((Pattern*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> LambdaArg::clone() {
        return unique_ptr<Node>((Node*)new LambdaArg(Id, Loc));
    }

    unique_ptr<Node> AlgebraicDT::clone() {
        auto Res = new AlgebraicDT(Name, TypeConstructor, vector<unique_ptr<Product>>(), Loc);

        for (auto &Prod: Sum) {
            auto Clone = Prod->clone().release();
            Res->Sum.push_back(unique_ptr<Product>((Product*)Clone));
        }

        return unique_ptr<Node>((Node*)Res);
    }

    unique_ptr<Node> Product::clone() {
        return unique_ptr<Node>((Node*) new Product(Constructor, Values, Loc));
    }

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
            Declaration(Loc),
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
        string Str("def " + Id + " : " + Signature.str() + "\n");

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

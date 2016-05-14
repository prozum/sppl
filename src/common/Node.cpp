#include "Node.h"
#include "Expression.h"
#include "Visitor.h"

using namespace std;
using namespace common;

void Program::accept(Visitor &V) { V.visit(*this); }
void Function::accept(Visitor &V) { V.visit(*this); }
void Case::accept(Visitor &V) { V.visit(*this); }
void LambdaArg::accept(Visitor &V) { V.visit(*this); }
void AlgebraicDT::accept(Visitor &V) { V.visit(*this); }
void Product::accept(Visitor &V) { V.visit(*this); }


Node::Node(Location Loc) : Loc(Loc) {}

Program::Program(vector<unique_ptr<Declaration>> Decls, Location Loc)
    : Node(Loc), Decls(move(Decls)) {}

Program::Program(unique_ptr<Expression> AnonFunc, Location Loc) : Node(Loc) {

    Decls.push_back(make_unique<Function>(move(AnonFunc)));
}

Declaration::Declaration(Location Loc) : Node(Loc) {}

Function::Function(unique_ptr<Expression> Expr)
    : Declaration(Expr->Loc), Id(ANON_FUNC_NAME),
      Signature(Type(TypeId::UNKNOWN)), Anon(true) {
    Cases.push_back(make_unique<Case>(
        move(Expr), nullptr, vector<unique_ptr<Pattern>>(), Expr->Loc));
}

Function::Function(string Id, Type Ty, Location Loc)
    : Declaration(Loc), Id(Id), Signature(Ty) {
}

Case::Case(unique_ptr<Expression> Expr, unique_ptr<Expression> When,
           vector<unique_ptr<Pattern>> Patterns, Location Loc)
    : Node(Loc), Expr(move(Expr)), When(move(When)), Patterns(move(Patterns)) {}

LambdaArg::LambdaArg(string Id, Location Loc) : Node(Loc), Id(Id) {}

AlgebraicDT::AlgebraicDT(string Name, vector<Type> TypeConstructor,
                         vector<unique_ptr<Product>> Sum, Location Loc)
    : Declaration(Loc), Name(Name), TypeConstructor(TypeConstructor),
      Sum(move(Sum)) {}

Product::Product(string Constructor, vector<Type> Values, Location Loc)
    : Node(Loc), Constructor(Constructor), Values(Values) {}


unique_ptr<Node> Node::clone() const { return unique_ptr<Node>(doClone()); }

unique_ptr<Declaration> Declaration::clone() const {
    return unique_ptr<Declaration>(doClone());
}

unique_ptr<Case> Case::clone() const { return unique_ptr<Case>(doClone()); }

unique_ptr<LambdaArg> LambdaArg::clone() const {
    return unique_ptr<LambdaArg>(doClone());
}

unique_ptr<Product> Product::clone() const {
    return unique_ptr<Product>(doClone());
}

Program *Program::doClone() const {
    auto Res = new Program(vector<unique_ptr<Declaration>>(), Loc);

    for (auto &Decl : Decls) {
        Res->Decls.push_back(Decl->clone());
    }

    return Res;
}

Function *Function::doClone() const {
    auto Res = new Function(Id, Signature, Loc);

    for (auto &Case : Cases) {
        Res->Cases.push_back(Case->clone());
    }

    return Res;
}

Case *Case::doClone() const {
    Case *Res;
    if (When) {
        Res = new Case(Expr->clone(), When->clone(),
                       vector<unique_ptr<Pattern>>(), Loc);
    } else {
        Res = new Case(Expr->clone(), nullptr, vector<unique_ptr<Pattern>>(),
                       Loc);
    }

    for (auto &Pat : Patterns) {
        Res->Patterns.push_back(Pat->clone());
    }

    return Res;
}

LambdaArg *LambdaArg::doClone() const { return new LambdaArg(Id, Loc); }

AlgebraicDT *AlgebraicDT::doClone() const {
    auto Res = new AlgebraicDT(Name, TypeConstructor,
                               vector<unique_ptr<Product>>(), Loc);

    for (auto &Prod : Sum) {
        Res->Sum.push_back(Prod->clone());
    }

    return Res;
}

Product *Product::doClone() const {
    return new Product(Constructor, Values, Loc);
}


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
        return "\t| " + strJoin(Patterns, " ") + " when " + When->str() +
               " = " + Expr->str();
    else
        return "\t| " + strJoin(Patterns, " ") + " = " + Expr->str();
}

string LambdaArg::str() { return Id; }

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

template <class T> string common::strJoin(T &List, const std::string JoinStr) {
    string Str;
    for (size_t i = 0; i < List.size(); ++i) {
        Str += List[i]->str();
        if (i + 1 != List.size())
            Str += JoinStr;
    }

    return Str;
}

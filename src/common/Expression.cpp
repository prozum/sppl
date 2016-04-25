#include "Expression.h"
#include "Visitor.h"

using namespace common;

Expression::Expression(Location Loc) : Node(Loc), RetTy(TypeId::UNKNOWN) {}

Expression::Expression(Type Ty, Location Loc) : Node(Loc), RetTy(Ty) {}

IdExpr::IdExpr(string Val, Location Loc) : Expression(Loc), Val(Val) {}

IntExpr::IntExpr(long Val, Location Loc)
    : Expression(Type(TypeId::INT), Loc), Val(Val) {}

FloatExpr::FloatExpr(double Val, Location Loc)
    : Expression(Type(TypeId::FLOAT), Loc), Val(Val) {}

CharExpr::CharExpr(char Val, Location Loc)
    : Expression(Type(TypeId::CHAR), Loc), Val(Val) {}

BoolExpr::BoolExpr(bool Val, Location Loc)
    : Expression(Type(TypeId::BOOL), Loc), Val(Val) {}

ListExpr::ListExpr(vector<unique_ptr<Expression>> Elements, Location Loc)
    : Expression(Type(TypeId::LIST), Loc), Elements(move(Elements)) {}

TupleExpr::TupleExpr(vector<unique_ptr<Expression>> Elements, Location Loc)
    : Expression(Type(TypeId::TUPLE), Loc), Elements(move(Elements)) {}

CallExpr::CallExpr(unique_ptr<Expression> Callee,
                   vector<unique_ptr<Expression>> Args, Location Loc)
    : Expression(Loc), Callee(move(Callee)), Args(move(Args)) {}

LambdaFunction::LambdaFunction(unique_ptr<Expression> Expr,
                               vector<unique_ptr<LambdaArg>> Args, Location Loc)
    : Expression(Loc), Expr(move(Expr)), Args(move(Args)) {}

AlgebraicExpr::AlgebraicExpr(string Constructor,
                             vector<unique_ptr<Expression>> Exprs, Location Loc)
    : Expression(Type(TypeId::CUSTOM), Loc), Constructor(Constructor),
      Exprs(move(Exprs)) {}

StringExpr::StringExpr(string Val, Location Loc)
    : Expression(Type(TypeId::STRING, Val.size() + 1), Loc), Val(Val) {}

void IdExpr::accept(Visitor &V) { V.visit(*this); }
void IntExpr::accept(Visitor &V) { V.visit(*this); }
void FloatExpr::accept(Visitor &V) { V.visit(*this); }
void CharExpr::accept(Visitor &V) { V.visit(*this); }
void BoolExpr::accept(Visitor &V) { V.visit(*this); }
void StringExpr::accept(Visitor &V) { V.visit(*this); }
void ListExpr::accept(Visitor &V) { V.visit(*this); }
void TupleExpr::accept(Visitor &V) { V.visit(*this); }
void CallExpr::accept(Visitor &V) { V.visit(*this); }
void LambdaFunction::accept(Visitor &V) { V.visit(*this); }
void AlgebraicExpr::accept(Visitor &V) { V.visit(*this); }

unique_ptr<Expression> Expression::clone() const {
    return unique_ptr<Expression>(doClone());
}

Expression *IdExpr::doClone() const { return new IdExpr(Val, Loc); }

Expression *IntExpr::doClone() const { return new IntExpr(Val, Loc); }

Expression *FloatExpr::doClone() const { return new FloatExpr(Val, Loc); }

Expression *CharExpr::doClone() const { return new CharExpr(Val, Loc); }

Expression *BoolExpr::doClone() const { return new BoolExpr(Val, Loc); }

Expression *StringExpr::doClone() const { return new StringExpr(Val, Loc); }

Expression *ListExpr::doClone() const {
    auto Res = new ListExpr(vector<unique_ptr<Expression>>(), Loc);

    for (auto &Element : Elements) {
        auto Ptr = Element->clone().release();
        Res->Elements.push_back(unique_ptr<Expression>((Expression *)Ptr));
    }

    return Res;
}

Expression *TupleExpr::doClone() const {
    auto Res = new TupleExpr(vector<unique_ptr<Expression>>(), Loc);

    for (auto &Element : Elements) {
        auto Ptr = Element->clone().release();
        Res->Elements.push_back(unique_ptr<Expression>((Expression *)Ptr));
    }

    return Res;
}

Expression *CallExpr::doClone() const {
    auto NewCallee = Callee->clone().release();
    auto Res = new CallExpr(unique_ptr<Expression>((Expression *)NewCallee),
                            vector<unique_ptr<Expression>>(), Loc);

    for (auto &Arg : Args) {
        auto Ptr = Arg->clone().release();
        Res->Args.push_back(unique_ptr<Expression>((Expression *)Ptr));
    }

    return Res;
}

Expression *LambdaFunction::doClone() const {
    auto Res =
        new LambdaFunction(Expr->clone(), vector<unique_ptr<LambdaArg>>(), Loc);

    for (auto &Arg : Args) {
        Res->Args.push_back(Arg->clone());
    }

    return Res;
}

Expression *AlgebraicExpr::doClone() const {
    auto Res =
        new AlgebraicExpr(Constructor, vector<unique_ptr<Expression>>(), Loc);

    for (auto &Expr : Exprs) {
        Res->Exprs.push_back(Expr->clone());
    }

    return Res;
}

string IdExpr::str() { return Val; }

string IntExpr::str() { return to_string(Val); }

string FloatExpr::str() { return to_string(Val); }

string CharExpr::str() { return string("\'") + Val + "\'"; }

string BoolExpr::str() { return (Val) ? "True" : "False"; }

string StringExpr::str() { return string("\"") + Val + "\""; }

string ListExpr::str() {
    // TODO fix strJoin problems. I give up for now
    string Str("[");

    for (auto &Element : Elements) {
        Str += Element->str();

        if (Element != Elements.back())
            Str += ", ";
    }

    return Str + "]";
}

string TupleExpr::str() {
    // TODO fix strJoin problems. I give up for now
    string Str("(");

    for (auto &Element : Elements) {
        Str += Element->str();

        if (Element != Elements.back())
            Str += ", ";
    }

    return Str + ")";
}

string CallExpr::str() {
    // TODO fix strJoin problems. I give up for now
    string Str(Callee->str() + "(");

    for (auto &Arg : Args) {
        Str += Arg->str();

        if (Arg != Args.back())
            Str += ", ";
    }

    return Str + ")";
}

string LambdaFunction::str() {
    // TODO fix strJoin problems. I give up for now
    string Str("\\");

    for (auto &Arg : Args) {
        Str += Arg->str();

        if (Arg != Args.back())
            Str += " ";
    }

    return Str + " => " + Expr->str();
}

string AlgebraicExpr::str() {
    // TODO fix strJoin problems. I give up for now
    string Str(Constructor);

    for (auto &Expr : Exprs) {
        Str += " " + Expr->str();
    }

    return Str;
}
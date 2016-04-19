#include "Expression.h"
#include "Visitor.h"


namespace common {
    Expression::Expression(Location Loc) :
            Node(Loc),
            RetTy(TypeId::UNKNOWN) { }

    Expression::Expression(Type Ty, Location Loc) :
            Node(Loc),
            RetTy(Ty) { }

    IdExpr::IdExpr(string Val, Location Loc) :
            Expression(Loc),
            Val(Val) { }

    IntExpr::IntExpr(long Val, Location Loc) :
            Expression(Type(TypeId::INT), Loc),
            Val(Val) { }

    FloatExpr::FloatExpr(double Val, Location Loc) :
            Expression(Type(TypeId::FLOAT), Loc),
            Val(Val) { }

    CharExpr::CharExpr(char Val, Location Loc) :
            Expression(Type(TypeId::CHAR), Loc),
            Val(Val) { }

    BoolExpr::BoolExpr(bool Val, Location Loc) :
            Expression(Type(TypeId::BOOL), Loc),
            Val(Val) { }

    ListExpr::ListExpr(vector<unique_ptr<Expression>> Elements,
                                   Location Loc) :
            Expression(Type(TypeId::LIST), Loc),
            Elements(move(Elements)) { }

    TupleExpr::TupleExpr(vector<unique_ptr<Expression>> Elements,
                                     Location Loc) :
            Expression(Type(TypeId::TUPLE), Loc),
            Elements(move(Elements)) { }

    CallExpr::CallExpr(unique_ptr<Expression> Callee,
               vector<unique_ptr<Expression>> Args,
               Location Loc) :
            Expression(Loc),
            Callee(move(Callee)),
            Args(move(Args)) { }

    LambdaFunction::LambdaFunction(unique_ptr<Expression> Expr,
                                   vector<unique_ptr<LambdaArg>> Args,
                                   Location Loc) :
            Expression(Loc),
            Expr(move(Expr)),
            Args(move(Args)) { }

    void IdExpr::accept(Visitor &V) { V.visit(*this); }
    void IntExpr::accept(Visitor &V) { V.visit(*this); }
    void FloatExpr::accept(Visitor &V) { V.visit(*this); }
    void CharExpr::accept(Visitor &V) { V.visit(*this); }
    void BoolExpr::accept(Visitor &V) { V.visit(*this); }
    void ListExpr::accept(Visitor &V) { V.visit(*this); }
    void TupleExpr::accept(Visitor &V) { V.visit(*this); }
    void CallExpr::accept(Visitor &V) { V.visit(*this); }
    void LambdaFunction::accept(Visitor &V) { V.visit(*this); }

    string IdExpr::str() {
        return Val;
    }

    string IntExpr::str() {
        return to_string(Val);
    }

    string FloatExpr::str(){
        return to_string(Val);
    }

    string CharExpr::str() {
        return string("\'") + Val + "\'";
    }

    string BoolExpr::str() {
        return (Val) ? "True" : "False";
    }

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

}
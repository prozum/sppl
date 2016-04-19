//
// Created by hejsil on 4/18/16.
//

#include "Expression.h"
#include "Visitor.h"


namespace common {
    Expression::Expression(Location Loc) :
            Node(Loc) { }

    Expression::Expression(Type Ty, Location Loc) :
            Node(Ty, Loc) { }

    IdExpression::IdExpression(string Val,
                               Location Loc) :
            Expression(Loc),
            Val(Val) { }


    IntExpression::IntExpression(long Val,
                                 Location Loc) :
            Expression(Loc),
            Val(Val) { }


    FloatExpression::FloatExpression(double Val,
                                     Location Loc) :
            Expression(Loc),
            Val(Val) { }


    CharExpression::CharExpression(char Val,
                                   Location Loc) :
            Expression(Loc),
            Val(Val) { }

    BoolExpression::BoolExpression(bool Val,
                                   Location Loc) :
            Expression(Loc),
            Val(Val) { }

    ListExpression::ListExpression(vector<unique_ptr<Expression>> Elements,
                                   Location Loc) :
            Expression(Type(TypeId::LIST), Loc),
            Elements(move(Elements)) { }

    TupleExpression::TupleExpression(vector<unique_ptr<Expression>> Elements,
                                     Location Loc) :
            Expression(Type(TypeId::TUPLE), Loc),
            Elements(move(Elements)) { }

    Call::Call(unique_ptr<Expression> Callee,
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

    void IdExpression::accept(Visitor &V) { V.visit(*this); }
    void IntExpression::accept(Visitor &V) { V.visit(*this); }
    void FloatExpression::accept(Visitor &V) { V.visit(*this); }
    void CharExpression::accept(Visitor &V) { V.visit(*this); }
    void BoolExpression::accept(Visitor &V) { V.visit(*this); }
    void ListExpression::accept(Visitor &V) { V.visit(*this); }
    void TupleExpression::accept(Visitor &V) { V.visit(*this); }
    void Call::accept(Visitor &V) { V.visit(*this); }
    void LambdaFunction::accept(Visitor &V) { V.visit(*this); }

    string IdExpression::str() {
        return Val;
    }

    string IntExpression::str() {
        return to_string(Val);
    }

    string FloatExpression::str(){
        return to_string(Val);
    }

    string CharExpression::str() {
        return string("\'") + Val + "\'";
    }

    string BoolExpression::str() {
        return (Val) ? "True" : "False";
    }

    string ListExpression::str() {
        // TODO fix strJoin problems. I give up for now
        string Str("[");

        for (auto &Element : Elements) {
            Str += Element->str();

            if (Element != Elements.back())
                Str += ", ";
        }

        return Str + "]";
    }

    string TupleExpression::str() {
        // TODO fix strJoin problems. I give up for now
        string Str("(");

        for (auto &Element : Elements) {
            Str += Element->str();

            if (Element != Elements.back())
                Str += ", ";
        }

        return Str + ")";
    }

    string Call::str() {
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
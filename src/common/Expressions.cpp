//
// Created by hejsil on 4/18/16.
//

#include "Expressions.h"
#include "Visitor.h"


namespace common {
    Expression::Expression(Location Loc) :
            Node(Loc) { }

    Expression::Expression(Type Ty, Location Loc) :
            Node(Ty, Loc) { }

    IdExpression::IdExpression(string Val,
                               Location Loc) :
            Val(Val),
            Expression(Loc) { }


    IntExpression::IntExpression(long Val,
                                 Location Loc) :
            Val(Val),
            Expression(Loc) { }


    FloatExpression::FloatExpression(double Val,
                                     Location Loc) :
            Val(Val),
            Expression(Loc) { }


    CharExpression::CharExpression(char Val,
                                   Location Loc) :
            Val(Val),
            Expression(Loc) { }

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

    void IdExpression::accept(Visitor &V) { V.visit(*this); }
    void IntExpression::accept(Visitor &V) { V.visit(*this); }
    void FloatExpression::accept(Visitor &V) { V.visit(*this); }
    void CharExpression::accept(Visitor &V) { V.visit(*this); }
    void ListExpression::accept(Visitor &V) { V.visit(*this); }
    void TupleExpression::accept(Visitor &V) { V.visit(*this); }
    void Call::accept(Visitor &V) { V.visit(*this); }

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
        return to_string(Val);
    }

    string ListExpression::str() {
        return "[" + strJoin(Elements, ", ") + "]";
    }

    string TupleExpression::str() {
        return "(" + strJoin(Elements, ", ") + ")";
    }

    string Call::str() {
        return Callee->str() + "(" + strJoin(Args, ", ") + ")";
    }

}
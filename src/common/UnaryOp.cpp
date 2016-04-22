#include "UnaryOp.h"
#include "Visitor.h"

namespace common {

    void ParExpr::accept(Visitor &V) { V.visit(*this); }
    void Not::accept(Visitor &V) { V.visit(*this); }
    void To::accept(Visitor &V) { V.visit(*this); }
    void Negative::accept(Visitor &V) { V.visit(*this); }

    unique_ptr<Node> ParExpr::clone() {
        auto NewChild = Child->clone().release();
        return unique_ptr<Node>((Node*)new ParExpr(unique_ptr<Expression>((Expression*)NewChild), Loc));
    }

    unique_ptr<Node> Not::clone() {
        auto NewChild = Child->clone().release();
        return unique_ptr<Node>((Node*)new Not(unique_ptr<Expression>((Expression*)NewChild), Loc));
    }

    unique_ptr<Node> To::clone() {
        auto NewChild = Child->clone().release();
        return unique_ptr<Node>((Node*)new To(unique_ptr<Expression>((Expression*)NewChild), TypeCast, Loc));
    }

    unique_ptr<Node> Negative::clone() {
        auto NewChild = Child->clone().release();
        return unique_ptr<Node>((Node*)new Negative(unique_ptr<Expression>((Expression*)NewChild), Loc));
    }

    UnaryOp::UnaryOp(unique_ptr<Expression> Child,
                 Location Loc) :
        Expression(Loc),
        Child(move(Child)) { }

    ParExpr::ParExpr(unique_ptr<Expression> Child,
             Location Loc) :
            UnaryOp(move(Child), Loc) { }

    Not::Not(unique_ptr<Expression> Child,
             Location Loc) :
            UnaryOp(move(Child), Loc) { }

    To::To(unique_ptr<Expression> Child,
           Type TypeCast,
           Location Loc) :
            UnaryOp(move(Child), Loc),
            TypeCast(TypeCast) { }

    Negative::Negative(unique_ptr<Expression> Child,
                       Location Loc) :
            UnaryOp(move(Child), Loc) { }

    string ParExpr::str() {
        return "(" + Child->str() + ")";
    }

    string Not::str() {
        return "!" + Child->str();
    }

    string Negative::str() {
        return "-" + Child->str();
    }

    string To::str() {
        // TODO Fix the Ty.str. Error "Ty must be static"
        return Child->str() + " to " /* + Ty.str() */;
    }

    template<class T>
    unique_ptr<Node> cloneUnaryOp(UnaryOp& Op) {
        auto Child = Op.Child->clone().release();
        auto Res = new T(unique_ptr<Expression>((Expression*)Child), Op.Loc);
        return unique_ptr<Node>((Node*)Res);
    }

}
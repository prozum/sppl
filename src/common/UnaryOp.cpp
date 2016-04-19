#include "UnaryOp.h"
#include "Visitor.h"

namespace common {

    void ParExpr::accept(Visitor &V) { V.visit(*this); }
    void Not::accept(Visitor &V) { V.visit(*this); }
    void To::accept(Visitor &V) { V.visit(*this); }
    void Negative::accept(Visitor &V) { V.visit(*this); }

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
           Location Loc) :
            UnaryOp(move(Child), Loc) { }

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

}
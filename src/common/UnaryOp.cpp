#include "UnaryOp.h"
#include "Visitor.h"

namespace common {

    void Par::accept(Visitor &V) { V.visit(*this); }
    void Not::accept(Visitor &V) { V.visit(*this); }

    UnaryOp::UnaryOp(unique_ptr<Expression> Child,
                 Location Loc) :
        Expression(Loc),
        Child(move(Child)) { }

    Par::Par(unique_ptr<Expression> Child,
             Location Loc) :
            UnaryOp(move(Child), Loc) { }

    Not::Not(unique_ptr<Expression> Child,
             Location Loc) :
            UnaryOp(move(Child), Loc) { }

    string Par::str() {
        return "(" + Child->str() + ")";
    }

    string Not::str() {
        return "!" + Child->str();
    }

}
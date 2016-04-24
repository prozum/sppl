#include "UnaryOp.h"
#include "Visitor.h"

namespace common {

    void ParExpr::accept(Visitor &V) { V.visit(*this); }
    void Not::accept(Visitor &V) { V.visit(*this); }
    void To::accept(Visitor &V) { V.visit(*this); }
    void Negative::accept(Visitor &V) { V.visit(*this); }

    unique_ptr<UnaryOp> UnaryOp::clone() const {
        return unique_ptr<UnaryOp>(doClone());
    }

    UnaryOp *ParExpr::doClone() const {
        return new ParExpr(Child->clone(), Loc);
    }

    UnaryOp *Not::doClone() const {
        return new Not(Child->clone(), Loc);
    }

    UnaryOp *To::doClone() const {
        return new To(Child->clone(), TypeCast, Loc);
    }

    UnaryOp *Negative::doClone() const {
        return new Negative(Child->clone(), Loc);
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
    T *const cloneUnaryOp(T& Op) {
        return new T(Op.Child->clone(), Op.Loc);
    }
}
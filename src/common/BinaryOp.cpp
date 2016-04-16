#include "BinaryOp.h"
#include "Visitor.h"

namespace common {

    void Add::accept(Visitor &v) { v.visit(*this); }
    void Sub::accept(Visitor &v) { v.visit(*this); }
    void Mul::accept(Visitor &v) { v.visit(*this); }
    void Div::accept(Visitor &v) { v.visit(*this); }
    void Mod::accept(Visitor &v) { v.visit(*this); }
    void ListAdd::accept(Visitor &v) { v.visit(*this); }
    void Or::accept(Visitor &v) { v.visit(*this); }
    void And::accept(Visitor &v) { v.visit(*this); }
    void Equal::accept(Visitor &v) { v.visit(*this); }
    void NotEqual::accept(Visitor &v) { v.visit(*this); }
    void Lesser::accept(Visitor &v) { v.visit(*this); }
    void Greater::accept(Visitor &v) { v.visit(*this); }
    void LesserEq::accept(Visitor &v) { v.visit(*this); }
    void GreaterEq::accept(Visitor &v) { v.visit(*this); }

    BinaryOp::BinaryOp(unique_ptr<Expression> left,
                   unique_ptr<Expression> right,
                   Location loc) :
        Expression(loc),
        Left(move(left)),
        Right(move(right)) { }

    Add::Add(unique_ptr<Expression> left,
         unique_ptr<Expression> right,
         Location loc) :
        BinaryOp(move(left), move(right), loc) { }

    Sub::Sub(unique_ptr<Expression> left,
             unique_ptr<Expression> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Mul::Mul(unique_ptr<Expression> left,
             unique_ptr<Expression> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Div::Div(unique_ptr<Expression> left,
             unique_ptr<Expression> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Mod::Mod(unique_ptr<Expression> left,
             unique_ptr<Expression> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    ListAdd::ListAdd(unique_ptr<Expression> left,
                     unique_ptr<Expression> right,
                     Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Or::Or(unique_ptr<Expression> left,
           unique_ptr<Expression> right,
           Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    And::And(unique_ptr<Expression> left,
             unique_ptr<Expression> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Equal::Equal(unique_ptr<Expression> left,
                 unique_ptr<Expression> right,
                 Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    NotEqual::NotEqual(unique_ptr<Expression> left,
                       unique_ptr<Expression> right,
                       Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Lesser::Lesser(unique_ptr<Expression> left,
                   unique_ptr<Expression> right,
                   Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Greater::Greater(unique_ptr<Expression> left,
                     unique_ptr<Expression> right,
                     Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    LesserEq::LesserEq(unique_ptr<Expression> left,
                       unique_ptr<Expression> right,
                       Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    GreaterEq::GreaterEq(unique_ptr<Expression> left,
                         unique_ptr<Expression> right,
                         Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    string Or::str() {
        return Left->str() + " || " + Right->str();
    }

    string And::str() {
        return Left->str() + " && " + Right->str();
    }

    string Equal::str() {
        return Left->str() + " == " + Right->str();
    }

    string NotEqual::str() {
        return Left->str() + " != " + Right->str();
    }

    string Lesser::str() {
        return Left->str() + " < " + Right->str();
    }

    string Greater::str() {
        return Left->str() + " > " + Right->str();
    }

    string LesserEq::str() {
        return Left->str() + " <= " + Right->str();
    }

    string GreaterEq::str() {
        return Left->str() + " >= " + Right->str();
    }

    string Add::str() {
        return Left->str() + " + " + Right->str();
    }

    string Sub::str() {
        return Left->str() + " - " + Right->str();
    }

    string Mul::str() {
        return Left->str() + " * " + Right->str();
    }

    string Div::str() {
        return Left->str() + " / " + Right->str();
    }

    string Mod::str() {
        return Left->str() + " % " + Right->str();
    }

    string ListAdd::str() {
        return Left->str() + " : " + Right->str();
    }

}

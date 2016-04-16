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

    BinaryOp::BinaryOp(unique_ptr<Expr> left,
                   unique_ptr<Expr> right,
                   Location loc) :
        Expr(loc),
        left(move(left)),
        right(move(right)) { }

    Add::Add(unique_ptr<Expr> left,
         unique_ptr<Expr> right,
         Location loc) :
        BinaryOp(move(left), move(right), loc) { }

    Sub::Sub(unique_ptr<Expr> left,
             unique_ptr<Expr> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Mul::Mul(unique_ptr<Expr> left,
             unique_ptr<Expr> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Div::Div(unique_ptr<Expr> left,
             unique_ptr<Expr> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Mod::Mod(unique_ptr<Expr> left,
             unique_ptr<Expr> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    ListAdd::ListAdd(unique_ptr<Expr> left,
                     unique_ptr<Expr> right,
                     Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Or::Or(unique_ptr<Expr> left,
           unique_ptr<Expr> right,
           Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    And::And(unique_ptr<Expr> left,
             unique_ptr<Expr> right,
             Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Equal::Equal(unique_ptr<Expr> left,
                 unique_ptr<Expr> right,
                 Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    NotEqual::NotEqual(unique_ptr<Expr> left,
                       unique_ptr<Expr> right,
                       Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Lesser::Lesser(unique_ptr<Expr> left,
                   unique_ptr<Expr> right,
                   Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    Greater::Greater(unique_ptr<Expr> left,
                     unique_ptr<Expr> right,
                     Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    LesserEq::LesserEq(unique_ptr<Expr> left,
                       unique_ptr<Expr> right,
                       Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    GreaterEq::GreaterEq(unique_ptr<Expr> left,
                         unique_ptr<Expr> right,
                         Location loc) :
            BinaryOp(move(left), move(right), loc) { }

    string Or::str() {
        return left->str() + " || " + right->str();
    }

    string And::str() {
        return left->str() + " && " + right->str();
    }

    string Equal::str() {
        return left->str() + " == " + right->str();
    }

    string NotEqual::str() {
        return left->str() + " != " + right->str();
    }

    string Lesser::str() {
        return left->str() + " < " + right->str();
    }

    string Greater::str() {
        return left->str() + " > " + right->str();
    }

    string LesserEq::str() {
        return left->str() + " <= " + right->str();
    }

    string GreaterEq::str() {
        return left->str() + " >= " + right->str();
    }

    string Add::str() {
        return left->str() + " + " + right->str();
    }

    string Sub::str() {
        return left->str() + " - " + right->str();
    }

    string Mul::str() {
        return left->str() + " * " + right->str();
    }

    string Div::str() {
        return left->str() + " / " + right->str();
    }

    string Mod::str() {
        return left->str() + " % " + right->str();
    }

    string ListAdd::str() {
        return left->str() + " : " + right->str();
    }

}

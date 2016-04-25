#include "BinaryOp.h"
#include "Expression.h"
#include "Visitor.h"

namespace common {

    void Add::accept(Visitor &V) { V.visit(*this); }
    void Sub::accept(Visitor &V) { V.visit(*this); }
    void Mul::accept(Visitor &V) { V.visit(*this); }
    void Div::accept(Visitor &v) { v.visit(*this); }
    void Mod::accept(Visitor &V) { V.visit(*this); }
    void ListAdd::accept(Visitor &V) { V.visit(*this); }
    void Or::accept(Visitor &V) { V.visit(*this); }
    void And::accept(Visitor &V) { V.visit(*this); }
    void Equal::accept(Visitor &V) { V.visit(*this); }
    void NotEqual::accept(Visitor &V) { V.visit(*this); }
    void Lesser::accept(Visitor &V) { V.visit(*this); }
    void Greater::accept(Visitor &V) { V.visit(*this); }
    void LesserEq::accept(Visitor &V) { V.visit(*this); }
    void GreaterEq::accept(Visitor &V) { V.visit(*this); }
    void ProducerConsumer::accept(Visitor &V) { V.visit(*this); }
    void Concat::accept(Visitor &V) { V.visit(*this); }

    BinaryOp::BinaryOp(unique_ptr<Expression> Left,
                   unique_ptr<Expression> Right,
                   Location Loc) :
        Expression(Loc),
        Left(move(Left)),
        Right(move(Right)) { }

    Add::Add(unique_ptr<Expression> Left,
         unique_ptr<Expression> Right,
         Location Loc) :
        BinaryOp(move(Left), move(Right), Loc) { }

    Sub::Sub(unique_ptr<Expression> Left,
             unique_ptr<Expression> Right,
             Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Mul::Mul(unique_ptr<Expression> Left,
             unique_ptr<Expression> Right,
             Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Div::Div(unique_ptr<Expression> Left,
             unique_ptr<Expression> Right,
             Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Mod::Mod(unique_ptr<Expression> Left,
             unique_ptr<Expression> Right,
             Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    ListAdd::ListAdd(unique_ptr<Expression> Left,
                     unique_ptr<Expression> Right,
                     Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Or::Or(unique_ptr<Expression> Left,
           unique_ptr<Expression> Right,
           Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    And::And(unique_ptr<Expression> Left,
             unique_ptr<Expression> Right,
             Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Equal::Equal(unique_ptr<Expression> Left,
                 unique_ptr<Expression> Right,
                 Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    NotEqual::NotEqual(unique_ptr<Expression> Left,
                       unique_ptr<Expression> Right,
                       Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Lesser::Lesser(unique_ptr<Expression> Left,
                   unique_ptr<Expression> Right,
                   Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Greater::Greater(unique_ptr<Expression> Left,
                     unique_ptr<Expression> Right,
                     Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    LesserEq::LesserEq(unique_ptr<Expression> Left,
                       unique_ptr<Expression> Right,
                       Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    GreaterEq::GreaterEq(unique_ptr<Expression> Left,
                         unique_ptr<Expression> Right,
                         Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    ProducerConsumer::ProducerConsumer(unique_ptr<Expression> Left,
                                           unique_ptr<Expression> Right,
                                           Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

    Concat::Concat(unique_ptr<Expression> Left,
                   unique_ptr<Expression> Right,
                   Location Loc) :
            BinaryOp(move(Left), move(Right), Loc) { }

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

    string ProducerConsumer::str() {
        return Left->str() + " |> " + Right->str();
    }

    string Concat::str() {
        return Left->str() + " ++ " + Right->str();
    }


    unique_ptr<Node> Or::clone() {
        return cloneBinaryOp<Or>(*this);
    }

    unique_ptr<Node> And::clone() {
        return cloneBinaryOp<And>(*this);
    }

    unique_ptr<Node> Equal::clone() {
        return cloneBinaryOp<Equal>(*this);
    }

    unique_ptr<Node> NotEqual::clone() {
        return cloneBinaryOp<NotEqual>(*this);
    }

    unique_ptr<Node> Lesser::clone() {
        return cloneBinaryOp<Lesser>(*this);
    }

    unique_ptr<Node> Greater::clone() {
        return cloneBinaryOp<Greater>(*this);
    }

    unique_ptr<Node> LesserEq::clone() {
        return cloneBinaryOp<LesserEq>(*this);
    }

    unique_ptr<Node> GreaterEq::clone() {
        return cloneBinaryOp<GreaterEq>(*this);
    }

    unique_ptr<Node> Add::clone() {
        return cloneBinaryOp<Add>(*this);
    }

    unique_ptr<Node> Sub::clone() {
        return cloneBinaryOp<Sub>(*this);
    }

    unique_ptr<Node> Mul::clone() {
        return cloneBinaryOp<Mul>(*this);
    }

    unique_ptr<Node> Div::clone() {
        return cloneBinaryOp<Div>(*this);
    }

    unique_ptr<Node> Mod::clone() {
        return cloneBinaryOp<Mod>(*this);
    }

    unique_ptr<Node> ListAdd::clone() {
        return cloneBinaryOp<ListAdd>(*this);
    }

    unique_ptr<Node> ProducerConsumer::clone() {
        return cloneBinaryOp<ProducerConsumer>(*this);
    }

    unique_ptr<Node> Concat::clone() {
        return cloneBinaryOp<Concat>(*this);
    }

    template<class T>
    unique_ptr<Node> cloneBinaryOp(BinaryOp& Op) {
        auto Left = Op.Left->clone().release();
        auto Right = Op.Right->clone().release();
        return unique_ptr<Node>((Node*)new T(unique_ptr<Expression>((Expression*)Left), unique_ptr<Expression>((Expression*)Right), Op.Loc));
    }
}

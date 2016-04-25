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

BinaryOp::BinaryOp(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
                   Location Loc)
    : Expression(Loc), Left(move(Left)), Right(move(Right)) {}

Add::Add(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
         Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Sub::Sub(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
         Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Mul::Mul(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
         Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Div::Div(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
         Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Mod::Mod(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
         Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

ListAdd::ListAdd(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
                 Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Or::Or(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

And::And(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
         Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Equal::Equal(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
             Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

NotEqual::NotEqual(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
                   Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Lesser::Lesser(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
               Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Greater::Greater(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
                 Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

LesserEq::LesserEq(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
                   Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

GreaterEq::GreaterEq(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
                     Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

ProducerConsumer::ProducerConsumer(unique_ptr<Expression> Left,
                                   unique_ptr<Expression> Right, Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

Concat::Concat(unique_ptr<Expression> Left, unique_ptr<Expression> Right,
               Location Loc)
    : BinaryOp(move(Left), move(Right), Loc) {}

string Or::str() { return Left->str() + " || " + Right->str(); }

string And::str() { return Left->str() + " && " + Right->str(); }

string Equal::str() { return Left->str() + " == " + Right->str(); }

string NotEqual::str() { return Left->str() + " != " + Right->str(); }

string Lesser::str() { return Left->str() + " < " + Right->str(); }

string Greater::str() { return Left->str() + " > " + Right->str(); }

string LesserEq::str() { return Left->str() + " <= " + Right->str(); }

string GreaterEq::str() { return Left->str() + " >= " + Right->str(); }

string Add::str() { return Left->str() + " + " + Right->str(); }

string Sub::str() { return Left->str() + " - " + Right->str(); }

string Mul::str() { return Left->str() + " * " + Right->str(); }

string Div::str() { return Left->str() + " / " + Right->str(); }

string Mod::str() { return Left->str() + " % " + Right->str(); }

string ListAdd::str() { return Left->str() + " : " + Right->str(); }

string ProducerConsumer::str() { return Left->str() + " |> " + Right->str(); }

string Concat::str() { return Left->str() + " ++ " + Right->str(); }

BinaryOp *Or::doClone() const { return cloneBinaryOp<Or>(*this); }

BinaryOp *And::doClone() const { return cloneBinaryOp<And>(*this); }

BinaryOp *Equal::doClone() const { return cloneBinaryOp<Equal>(*this); }

BinaryOp *NotEqual::doClone() const { return cloneBinaryOp<NotEqual>(*this); }

BinaryOp *Lesser::doClone() const { return cloneBinaryOp<Lesser>(*this); }

BinaryOp *Greater::doClone() const { return cloneBinaryOp<Greater>(*this); }

BinaryOp *LesserEq::doClone() const { return cloneBinaryOp<LesserEq>(*this); }

BinaryOp *GreaterEq::doClone() const { return cloneBinaryOp<GreaterEq>(*this); }

BinaryOp *Add::doClone() const { return cloneBinaryOp<Add>(*this); }

BinaryOp *Sub::doClone() const { return cloneBinaryOp<Sub>(*this); }

BinaryOp *Mul::doClone() const { return cloneBinaryOp<Mul>(*this); }

BinaryOp *Div::doClone() const { return cloneBinaryOp<Div>(*this); }

BinaryOp *Mod::doClone() const { return cloneBinaryOp<Mod>(*this); }

BinaryOp *ListAdd::doClone() const { return cloneBinaryOp<ListAdd>(*this); }

BinaryOp *ProducerConsumer::doClone() const {
    return cloneBinaryOp<ProducerConsumer>(*this);
}

BinaryOp *Concat::doClone() const { return cloneBinaryOp<Concat>(*this); }

template <class T> T *cloneBinaryOp(const T &Op) {
    auto Left = Op.Left->clone();
    auto Right = Op.Right->clone();
    return new T(Op.Left->clone(), Op.Right->clone(), Op.Loc);
}
}

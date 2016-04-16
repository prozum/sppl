#include "UnaryOp.h"
#include "Visitor.h"

namespace common {

    void Par::accept(Visitor &v) { v.visit(*this); }
    void Not::accept(Visitor &v) { v.visit(*this); }

    UnaryOp::UnaryOp(unique_ptr<Expression> child,
                 Location loc) :
        Expression(loc),
        Child(move(child)) { }

    Par::Par(unique_ptr<Expression> child,
             Location loc) :
            UnaryOp(move(child), loc) { }

    Not::Not(unique_ptr<Expression> child,
             Location loc) :
            UnaryOp(move(child), loc) { }

    string Par::str() {
        return "(" + Child->str() + ")";
    }

    string Not::str() {
        return "!" + Child->str();
    }

}
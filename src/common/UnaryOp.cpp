#include "UnaryOp.h"
#include "Visitor.h"

namespace common {

    void Par::accept(Visitor &v) { v.visit(*this); }
    void Not::accept(Visitor &v) { v.visit(*this); }

    UnaryOp::UnaryOp(unique_ptr<Expr> child,
                 Location loc) :
        Expr(loc),
        child(move(child)) { }

    Par::Par(unique_ptr<Expr> child,
             Location loc) :
            UnaryOp(move(child), loc) { }

    Not::Not(unique_ptr<Expr> child,
             Location loc) :
            UnaryOp(move(child), loc) { }

    string Par::str() {
        return "(" + child->str() + ")";
    }

    string Not::str() {
        return "!" + child->str();
    }

}
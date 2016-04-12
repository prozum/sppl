#include "UnaryOp.h"
#include "Visitor.h"

namespace common {
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

    void Par::accept(Visitor &v) { v.visit(*this); }
    void Not::accept(Visitor &v) { v.visit(*this); }
}
#pragma once

#include "Common.h"
#include "Error.h"

#include <string>
#include <vector>

namespace common {
class Visitor {
    friend class Node;

  public:
    std::vector<Error> Errors;
    void addError(Error Err);
    bool hasError();

    virtual void visit(Program &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Function &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(AlgebraicDT &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Product &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Case &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(LambdaArg &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Or &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(And &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Equal &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(NotEqual &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Lesser &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Greater &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(LesserEq &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(GreaterEq &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Add &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Sub &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Mul &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Div &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Mod &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(ListAdd &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(ProducerConsumer &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Concat &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(To &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(ParExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Not &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(Negative &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(LambdaFunction &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(IdPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(IntPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(FloatPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(CharPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(BoolPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(StringPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(ListPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(TuplePattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(ListSplit &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(WildPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(AlgebraicPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(ParPattern &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(IdExpr &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(IntExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(FloatExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(CharExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(BoolExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(StringExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(ListExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(TupleExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(CallExpr &Node) {
        throw runtime_error("Not implemented");
    }
    virtual void visit(Type &Node) { throw runtime_error("Not implemented"); }
    virtual void visit(AlgebraicExpr &Node) {
        throw runtime_error("Not implemented");
    }
};
}
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
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Function &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(AlgebraicDT &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Product &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Case &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(LambdaArg &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Or &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(And &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Equal &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(NotEqual &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Lesser &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Greater &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(LesserEq &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(GreaterEq &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Add &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Sub &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Mul &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Div &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Mod &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ListAdd &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ProducerConsumer &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Concat &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(To &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ParExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Not &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Negative &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(LambdaFunction &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(IdPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(IntPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(FloatPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(CharPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(BoolPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(StringPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ListPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(TuplePattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ListSplit &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(WildPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(AlgebraicPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ParPattern &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(IdExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(IntExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(FloatExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(CharExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(BoolExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(StringExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(ListExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(TupleExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(CallExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(Type &Node) {
        throw std::runtime_error("Not implemented");
    }
    virtual void visit(AlgebraicExpr &Node) {
        throw std::runtime_error("Not implemented");
    }
};
}
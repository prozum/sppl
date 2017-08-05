#pragma once

#include "Common.h"
#include "Error.h"

#include <string>
#include <vector>

namespace common {
class Visitor {
    friend class Node;

  public:
    virtual ~Visitor() = default;

    void addError(Error Err);
    void clearErrors();
    bool checkNotSafe();
    void setSafe();
    size_t countErrors();
    std::vector<Error> getErrors();

    virtual void visit(Program &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Function &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(AlgebraicDT &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Product &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Case &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(LambdaArg &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Or &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(And &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Equal &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(NotEqual &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Lesser &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Greater &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(LesserEq &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(GreaterEq &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Add &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Sub &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Mul &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Div &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Mod &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ListAdd &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ProducerConsumer &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Concat &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(To &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(UnPrint &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ParExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Not &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Negative &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(DoExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Assosiate &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(LambdaFunction &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(IdPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(IntPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(FloatPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(CharPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(BoolPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(StringPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ListPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(TuplePattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ListSplit &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(WildPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(AlgebraicPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ParPattern &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(IdExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(IntExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(FloatExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(CharExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(BoolExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(StringExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(ListExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(TupleExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(CallExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(Type &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }
    virtual void visit(AlgebraicExpr &Node) {
        addError(Error::NotImplemented(Node.Loc));
    }

private:
    std::vector<Error> Errors;
    bool Safe = true;
};
}

#pragma once

#include "Expression.h"
#include <memory>

namespace common {

class UnaryOp : public Expression {
  public:
    std::unique_ptr<Expression> Child;

    UnaryOp(std::unique_ptr<Expression> Child, Location Loc);

    virtual void accept(Visitor &V) = 0;

    std::unique_ptr<UnaryOp> clone() const;

  private:
    virtual UnaryOp *doClone() const = 0;
};

class ParExpr : public UnaryOp {
  public:
    ParExpr(std::unique_ptr<Expression> Child, Location Loc);

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual UnaryOp *doClone() const;
};

class Not : public UnaryOp {
  public:
    Not(std::unique_ptr<Expression> Child, Location Loc);

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual UnaryOp *doClone() const;
};

class Negative : public UnaryOp {
  public:
    Negative(std::unique_ptr<Expression> Child, Location Loc);

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual UnaryOp *doClone() const;
};

class To : public UnaryOp {
  public:
    Type TypeCast;

    To(std::unique_ptr<Expression> Child, Type TypeCast, Location Loc);

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual UnaryOp *doClone() const;
};

class Assosiate : public UnaryOp {
public:
    std::string Id;

    Assosiate(std::unique_ptr<Expression> Child, std::string Id, Location Loc);

    virtual void accept(Visitor &V);
    std::string str();

private:
    virtual UnaryOp *doClone() const;
};

class UnPrint : public UnaryOp {
public:
    UnPrint(std::unique_ptr<Expression> Child, Location Loc);

    virtual void accept(Visitor &V);
    std::string str();

private:
    virtual UnaryOp *doClone() const;
};

template <class T> std::unique_ptr<Node> const cloneUnaryOp(UnaryOp &Op);
}
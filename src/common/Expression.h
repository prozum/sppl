#pragma once

#include "Node.h"

namespace common {
class Expression : public Node {
  public:
    Type RetTy;
    bool Const;

    Expression(Location Loc, bool Const = false);
    Expression(Type Ty, Location Loc, bool Const = false);
    virtual ~Expression() = default;

    virtual void accept(Visitor &V) = 0;

    std::unique_ptr<Expression> clone() const;

  private:
    virtual Expression *doClone() const = 0;
};

class IdExpr : public Expression {
  public:
    std::string Val;

    IdExpr(std::string Val, Location Loc);
    virtual ~IdExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class IntExpr : public Expression {
  public:
    long Val;

    IntExpr(long Val, Location Loc);
    virtual ~IntExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class FloatExpr : public Expression {
  public:
    double Val;

    FloatExpr(double Val, Location Loc);
    virtual ~FloatExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class CharExpr : public Expression {
  public:
    char Val;

    CharExpr(char Val, Location Loc);
    virtual ~CharExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class BoolExpr : public Expression {
  public:
    bool Val;

    BoolExpr(bool Val, Location Loc);
    virtual ~BoolExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class StringExpr : public Expression {
  public:
    std::string Val;

    StringExpr(std::string Val, Location Loc);
    virtual ~StringExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

/* Other Expressions */
class ListExpr : public Expression {
  public:
    std::vector<std::unique_ptr<Expression>> Elements;

    ListExpr(std::vector<std::unique_ptr<Expression>> Elements, Location Loc);
    virtual ~ListExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class TupleExpr : public Expression {
  public:
    std::vector<std::unique_ptr<Expression>> Elements;

    TupleExpr(std::vector<std::unique_ptr<Expression>> Elements, Location Loc);
    virtual ~TupleExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class CallExpr : public Expression {
  public:
    bool DoParallel = true;

    std::unique_ptr<Expression> Callee;
    std::vector<std::unique_ptr<Expression>> Args;

    CallExpr(std::unique_ptr<Expression> Callee, std::vector<std::unique_ptr<Expression>> Args,
             Location Loc);
    virtual ~CallExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class LambdaFunction : public Expression {
  public:
    std::unique_ptr<Expression> Expr;
    std::vector<std::unique_ptr<LambdaArg>> Args;

    LambdaFunction(std::unique_ptr<Expression> Expr,
                   std::vector<std::unique_ptr<LambdaArg>> Args, Location Loc);
    virtual ~LambdaFunction() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class AlgebraicExpr : public Expression {
  public:
    std::string Constructor;
    std::vector<std::unique_ptr<Expression>> Exprs;

    AlgebraicExpr(std::string Constructor, std::vector<std::unique_ptr<Expression>> Exprs,
                  Location Loc);
    virtual ~AlgebraicExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

  private:
    virtual Expression *doClone() const;
};

class DoExpr : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> Exprs;

    DoExpr(std::vector<std::unique_ptr<Expression>> Exprs,
           Location Loc);
    virtual ~DoExpr() = default;

    virtual void accept(Visitor &V);
    std::string str();

private:
    virtual Expression *doClone() const;
};
}

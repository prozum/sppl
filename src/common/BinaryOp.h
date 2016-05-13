#pragma once

#include "Expression.h"
#include "Node.h"

namespace common {

class BinaryOp : public Expression {
  public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;

    BinaryOp(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
             Location Loc);

    virtual void accept(Visitor &V) = 0;

    std::unique_ptr<BinaryOp> clone() const;

  private:
    virtual BinaryOp *doClone() const = 0;
};

class Add : public BinaryOp {
  public:
    Add(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
        Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Sub : public BinaryOp {
  public:
    Sub(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
        Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Mul : public BinaryOp {
  public:
    Mul(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
        Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Div : public BinaryOp {
  public:
    Div(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
        Location Loc);

    virtual void accept(Visitor &v);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Mod : public BinaryOp {
  public:
    Mod(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
        Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class ListAdd : public BinaryOp {
  public:
    ListAdd(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
            Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Or : public BinaryOp {
  public:
    Or(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right, Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class And : public BinaryOp {
  public:
    And(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
        Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Equal : public BinaryOp {
  public:
    Equal(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
          Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class NotEqual : public BinaryOp {
  public:
    NotEqual(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
             Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Lesser : public BinaryOp {
  public:
    Lesser(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
           Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Greater : public BinaryOp {
  public:
    Greater(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
            Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class LesserEq : public BinaryOp {
  public:
    LesserEq(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
             Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class GreaterEq : public BinaryOp {
  public:
    GreaterEq(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
              Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class ProducerConsumer : public BinaryOp {
  public:
    ProducerConsumer(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
                     Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class Concat : public BinaryOp {
  public:
    Concat(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
           Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

  private:
    virtual BinaryOp *doClone() const;
};

class BinPrint : public BinaryOp {
public:
    BinPrint(std::unique_ptr<Expression> Left, std::unique_ptr<Expression> Right,
           Location Loc);

    virtual void accept(Visitor &V);

    std::string str();

private:
    virtual BinaryOp *doClone() const;
};

template <class T> T *cloneBinaryOp(const T &Op);
}

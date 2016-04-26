#pragma once
#include "Location.h"
#include "Type.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#define ANON_FUNC_NAME "__anon_func"

namespace common {

// Forward declarations
class Node;

class Expression;

class Declaration;

class BinaryOp;

class UnaryOp;

class Type;

class Pattern;

class Program;

class Function;

class AlgebraicDT;

class Product;

class Case;

class LambdaArg;

class Or;

class And;

class Equal;

class NotEqual;

class Lesser;

class Greater;

class LesserEq;

class GreaterEq;

class Add;

class Sub;

class Mul;

class Div;

class Mod;

class ListAdd;

class ParExpr;

class Not;

class Negative;

class ProducerConsumer;

class Concat;

class LambdaFunction;

class IntPattern;

class FloatPattern;

class CharPattern;

class BoolPattern;

class StringPattern;

class ListPattern;

class TuplePattern;

class ListSplit;

class WildPattern;

class AlgebraicPattern;

class ParPattern;

class IntExpr;

class FloatExpr;

class CharExpr;

class BoolExpr;

class StringExpr;

class ListExpr;

class TupleExpr;

class IdPattern;

class CallExpr;

class AlgebraicExpr;

class Visitor;

class Scope;

// Abstract Nodes
class Node {
  public:
    Location Loc;

    Node(Location Loc);

    Node(const Node &Other);

    // virtual Node &operator=(const Node &Other) = 0;
    ~Node() = default;

    std::unique_ptr<Node> clone() const;

    virtual void accept(Visitor &V) = 0;

    virtual std::string str() = 0;

  private:
    virtual Node *doClone() const = 0;
};

class Program : public Node {
  public:
    std::vector<std::unique_ptr<Declaration>> Decls;

    Program(std::vector<std::unique_ptr<Declaration>> Decls, Location Loc);

    Program(std::unique_ptr<Expression> AnonFunc, Location Loc);

    void accept(Visitor &V);

    std::string str();

    std::unique_ptr<Program> clone() const;

  private:
    virtual Program *doClone() const;
};

/* Declaration */

class Declaration : public Node {
  public:
    Declaration(Location Loc);

    std::unique_ptr<Declaration> clone() const;

  private:
    virtual Declaration *doClone() const = 0;
};

class Function : public Declaration {
  public:
    std::string Id;
    Type Signature;
    std::vector<std::unique_ptr<Case>> Cases;
    Scope *Scp;
    bool Anon = false;

    Function(std::unique_ptr<Expression> AnonFunc);

    Function(std::string Id, Type Ty, Location Loc);

    void accept(Visitor &V);

    std::string str();

  private:
    virtual Function *doClone() const;
};

class AlgebraicDT : public Declaration {
  public:
    std::string Name;
    std::vector<Type> TypeConstructor;
    std::vector<std::unique_ptr<Product>> Sum;

    AlgebraicDT(std::string Name, std::vector<Type> TypeConstructor,
                std::vector<std::unique_ptr<Product>> Sum, Location Loc);

    void accept(Visitor &V);

    std::string str();

    std::unique_ptr<AlgebraicDT> clone() const;

  private:
    virtual AlgebraicDT *doClone() const;
};

class Product : public Node {
  public:
    AlgebraicDT *Parent = nullptr;

    std::string Constructor;
    std::vector<Type> Values;

    Product(std::string Constructor, std::vector<Type> Values, Location Loc);

    void accept(Visitor &V);

    std::string str();

    std::unique_ptr<Product> clone() const;

  private:
    virtual Product *doClone() const;
};

class Case : public Node {
  public:
    std::unique_ptr<Expression> Expr;
    std::unique_ptr<Expression> When;
    std::vector<std::unique_ptr<Pattern>> Patterns;
    bool TailRec = false;

    Case(std::unique_ptr<Expression> Expr, std::unique_ptr<Expression> When,
         std::vector<std::unique_ptr<Pattern>> Patterns, Location Loc);

    void accept(Visitor &V);

    std::string str();

    std::unique_ptr<Case> clone() const;

  private:
    virtual Case *doClone() const;
};

class LambdaArg : public Node {
  public:
    std::string Id;
    Scope *Scp;

    LambdaArg(std::string Id, Location Loc);

    void accept(Visitor &V);

    std::string str();

    std::unique_ptr<LambdaArg> clone() const;

  private:
    virtual LambdaArg *doClone() const;
};

template <class T> std::string strJoin(T &List, const std::string JoinStr);
}
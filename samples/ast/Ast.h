#ifndef AST_HEADER
#define AST_HEADER

#include <vector>

namespace ast {
    class Visitor;
    class Node;
    class Expr;
    class BinaryOp;
    class UnaryOp;
    class Type;
    class Program;
    class Function;
    class Case;
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
    class Par;
    class Not;
    class Negative;
    class Int;
    class Float;
    class Bool;
    class Char;
    class String;
    class List;
    class Tuple;
    class Id;
    class Call;
    class IntType;
    class FloatType;
    class BoolType;
    class CharType;
    class StringType;
    class ListType;
    class TupleType;
    class Signature;

    class Visitor {
    public:
        virtual void visit(Program &node) = 0;
        virtual void visit(Function &node) = 0;
        virtual void visit(Case &node) = 0;

        virtual void visit(Or &node) = 0;
        virtual void visit(And &node) = 0;
        virtual void visit(Equal &node) = 0;
        virtual void visit(NotEqual &node) = 0;
        virtual void visit(Lesser &node) = 0;
        virtual void visit(Greater &node) = 0;
        virtual void visit(LesserEq &node) = 0;
        virtual void visit(GreaterEq &node) = 0;
        virtual void visit(Add &node) = 0;
        virtual void visit(Sub &node) = 0;
        virtual void visit(Mul &node) = 0;
        virtual void visit(Div &node) = 0;
        virtual void visit(Mod &node) = 0;
        virtual void visit(ListAdd &node) = 0;
        virtual void visit(Par &node) = 0;
        virtual void visit(Not &node) = 0;
        virtual void visit(Negative &node) = 0;

        virtual void visit(Int &node) = 0;
        virtual void visit(Float &node) = 0;
        virtual void visit(Bool &node) = 0;
        virtual void visit(Char &node) = 0;
        virtual void visit(String &node) = 0;
        virtual void visit(List &node) = 0;
        virtual void visit(Tuple &node) = 0;
        virtual void visit(Id &node) = 0;
        virtual void visit(Call &node) = 0;

        virtual void visit(IntType &node) = 0;
        virtual void visit(FloatType &node) = 0;
        virtual void visit(BoolType &node) = 0;
        virtual void visit(CharType &node) = 0;
        virtual void visit(StringType &node) = 0;
        virtual void visit(ListType &node) = 0;
        virtual void visit(TupleType &node) = 0;
        virtual void visit(Signature &node) = 0;
    };

    /* Abstract Nodes */

    class Node {
    public:
        virtual void accept(Visitor& v) = 0;
    };

    class Expr : Node {
    public:
        virtual void accept(Visitor& v) = 0;
    };

    class BinaryOp : Node {
    public:
        Expr &left;
        Expr &right;

        virtual void accept(Visitor& v) = 0;
    };

    class UnaryOp : Node {
    public:
        Expr &child;

        virtual void accept(Visitor& v) = 0;
    };

    class Type : Node {
    public:
        virtual void accept(Visitor& v) = 0;
    };

    /* Declaration */

    class Program : Node {
    public:
        std::vector<Function> funcs;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Function : Node {
    public:
        std::string id;
        Signature &sig;
        std::vector<Case> cases;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Case : Node {
    public:
        std::vector<Expr> patterns;
        Expr &expr;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    /* Binary Operators */

    class Or : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class And : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Equal : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class NotEqual : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Lesser : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Greater : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class LesserEq : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class GreaterEq : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Add : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Sub : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Mul : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Div : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Mod : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class ListAdd : BinaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    /* Unary Operators */

    class Par : UnaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Not : UnaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Negative : UnaryOp {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    /* Literals */

    class Int : Expr {
    public:
        int value;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Float : Expr {
    public:
        float value;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Bool : Expr {
    public:
        bool value;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Char : Expr {
    public:
        char value;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class String : Expr {
    public:
        std::string value;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    /* Other Expressions */

    class List : Expr {
    public:
        std::vector<Expr> exprs;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Tuple : Expr {
    public:
        std::vector<Expr> exprs;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Id : Expr {
        std::string id;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Call : Expr {
    public:
        Expr &callee;
        std::vector<Expr> exprs;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    /* Types */

    class IntType : Type {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class FloatType : Type {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class BoolType : Type {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class CharType : Type {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class StringType : Type {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class ListType : Type {
    public:
        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class TupleType : Type {
    public:
        std::vector<Type> types;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };

    class Signature : Type {
    public:
        std::vector<Type> types;

        virtual void accept(Visitor& v) { v.visit(*this); };
    };
}

#endif

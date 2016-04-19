#pragma once

#include "Node.h"

namespace common {
    class Expression : public Node {
    public:
        Type RetTy;

        Expression(Location Loc);
        Expression(Type Ty, Location Loc);

        virtual void accept(Visitor &V) = 0;
    };

    class IdExpr : public Expression {
    public:
        string Val;
        Scope* Scp;
    
        IdExpr(string Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };
    
    class IntExpr : public Expression {
    public:
        long Val;
    
        IntExpr(long Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };
    
    class FloatExpr : public Expression {
    public:
        double Val;
    
        FloatExpr(double Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };
    
    class CharExpr : public Expression {
    public:
        char Val;
    
        CharExpr(char Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };

    class BoolExpr : public Expression {
    public:
        bool Val;

        BoolExpr(bool Val, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };

    /* Other Expressions */

    class ListExpr : public Expression {
    public:
        vector<unique_ptr<Expression>> Elements;

        ListExpr(vector<unique_ptr<Expression>> Elements, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };

    class TupleExpr : public Expression {
    public:
        vector<unique_ptr<Expression>> Elements;

        TupleExpr(vector<unique_ptr<Expression>> Elements, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };

    class CallExpr : public Expression {
    public:
        unique_ptr<Expression> Callee;
        vector<unique_ptr<Expression>> Args;

        CallExpr(unique_ptr<Expression> Callee, vector<unique_ptr<Expression>> Args, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };

    class LambdaFunction : public Expression {
    public:
        unique_ptr<Expression> Expr;
        vector<unique_ptr<LambdaArg>> Args;

        LambdaFunction(unique_ptr<Expression> Expr, vector<unique_ptr<LambdaArg>> Args, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };
}

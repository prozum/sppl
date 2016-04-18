#pragma once

#include "Node.h"

namespace common {
    class Expression : public Node {
    public:
        Expression(Location Loc);
        Expression(Type Ty, Location Loc);

        virtual void accept(Visitor &V) = 0;
    };

    class IdExpression : public Expression {
    public:
        string Val;
        Scope* Scp;
    
        IdExpression(string Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };
    
    class IntExpression : public Expression {
    public:
        long Val;
    
        IntExpression(long Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };
    
    class FloatExpression : public Expression {
    public:
        double Val;
    
        FloatExpression(double Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };
    
    class CharExpression : public Expression {
    public:
        char Val;
    
        CharExpression(char Val, Location Loc);
    
        virtual void accept(Visitor &V);
        string str();
    };

    /* Other Expressions */

    class ListExpression : public Expression {
    public:
        vector<unique_ptr<Expression>> Elements;

        ListExpression(vector<unique_ptr<Expression>> Elements, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };

    class TupleExpression : public Expression {
    public:
        vector<unique_ptr<Expression>> Elements;

        TupleExpression(vector<unique_ptr<Expression>> Elements, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };

    class Call : public Expression {
    public:
        unique_ptr<Expression> Callee;
        vector<unique_ptr<Expression>> Args;

        Call(unique_ptr<Expression> Callee, vector<unique_ptr<Expression>> Args, Location Loc);

        virtual void accept(Visitor &V);
        string str();
    };
}

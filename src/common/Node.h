#pragma once
#include "Type.h"
#include "Location.h"

#include <vector>
#include <string>
#include <memory>

using namespace std;

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
	class IntExpr;
	class FloatExpr;
	class CharExpr;
	class BoolExpr;
	class ListExpr;
	class TupleExpr;
	class IdPattern;
	class CallExpr;
	class Visitor;
	class Scope;

	// Abstract Nodes
	class Node {
	public:
		Location Loc;

		Node(Location Loc);
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        ~Node() = default;

		virtual void accept(Visitor &V) = 0;
		virtual string str() = 0;
	};

	class Program : public Node {
	public:
		vector<unique_ptr<Declaration>> Decls;

		Program(vector<unique_ptr<Declaration>> Decls, Location Loc);
		Program(unique_ptr<Expression> AnonFunc, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	/* Declaration */

	class Declaration : public Node {
	public:
		Declaration(Location Loc);
		Declaration(Type Ty, Location Loc);

		virtual void accept(Visitor &V) = 0;
		virtual string str() = 0;
	};

	class Function : public Declaration {
	public:
		string Id;
		Type Signature;
        vector<unique_ptr<Case>> Cases;
		Scope* Scp;
		bool Anon = false;

		Function(unique_ptr<Expression> AnonFunc);
		Function(string Id, Type Ty, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	// TODO Implement ADT
	class AlgebraicDT : public Declaration {
	public:
		string Name;
		vector<Type> TypeConstructor;
		vector<unique_ptr<Product>> Sum;

		AlgebraicDT(string Name, vector<Type> TypeConstructor, vector<unique_ptr<Product>> Sum, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Product : public Node {
	public:
		string Constructor;
		vector<Type> Values;

		Product(string Constructor, vector<Type> Values, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Case : public Node {
	public:
		unique_ptr<Expression> Expr;
		unique_ptr<Expression> When;
		vector<unique_ptr<Pattern>> Patterns;
		bool TailRec = false;

		Case(unique_ptr<Expression> Expr, unique_ptr<Expression> When, vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class LambdaArg : public Node {
	public:
		string Id;

		LambdaArg(string Id, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	template<class T>
	string strJoin(T &List, const std::string JoinStr);
}



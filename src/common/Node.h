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
	class BinaryOp;
	class UnaryOp;
	class Type;
	class Pattern;
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
	class Int;
	class Float;
	class Bool;
	class Char;
	class String;
	class ListPattern;
	class TuplePattern;
	class ListSplit;
	class List;
	class Tuple;
	class Id;
	class Call;
	class Visitor;
	class Scope;

	// Abstract Nodes
	class Node {
	public:
		Type Ty;
		Location Loc;

		Node(Location Loc);
		Node(Type Ty, Location Loc);
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        ~Node() = default;

		virtual void accept(Visitor &V) = 0;
		virtual string str() = 0;
	};

	class Program : public Node {
	public:
		vector<unique_ptr<Function>> Funcs;

		Program(vector<unique_ptr<Function>> Funcs, Location Loc);
		Program(unique_ptr<Expression> AnonFunc, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Expression : public Node {
	public:
		Expression(Location Loc);
		Expression(Type Ty, Location Loc);

		virtual void accept(Visitor &V) = 0;
	};

	/* Declaration */

	class Function : public Node {
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

	class Case : public Node {
	public:
		unique_ptr<Expression> Expr;
		vector<unique_ptr<Pattern>> Patterns;
		bool TailRec = false;

		Case(unique_ptr<Expression> Expr, vector<unique_ptr<Pattern>> Patterns, Location Loc);

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

	/* Other Expressions */

	class List : public Expression {
	public:
		vector<unique_ptr<Expression>> Elements;

		List(vector<unique_ptr<Expression>> Elements, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Tuple : public Expression {
	public:
		vector<unique_ptr<Expression>> Elements;

		Tuple(vector<unique_ptr<Expression>> Elements, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};
}



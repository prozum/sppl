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
	class ADT;
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
	class ProducerConsumer;
	class Concat;
	class IntPattern;
	class FloatPattern;
	class CharPattern;
	class ListPattern;
	class TuplePattern;
	class ListSplit;
	class WildPattern;
	class IntExpression;
	class FloatExpression;
	class CharExpression;
	class ListExpression;
	class TupleExpression;
	class IdPattern;
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
		vector<unique_ptr<Declaration>> Funcs;

		Program(vector<unique_ptr<Function>> Funcs, Location Loc);
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
	class ADT : public Declaration {
		ADT(Location Loc) : Declaration(Loc) { }

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


	template<class T>
	string strJoin(T &List, const std::string JoinStr);
}



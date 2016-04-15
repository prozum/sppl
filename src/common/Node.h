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
	class Expr;
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
		Type type;
		Location loc;

		Node(Location);
		Node(Type, Location);
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        ~Node() = default;

		virtual string str() = 0;
		virtual void accept(Visitor &) = 0;
	};

	class Program : public Node {
	public:

		vector<unique_ptr<Function>> funcs;

		Program(vector<unique_ptr<Function>>, Location);
		Program(unique_ptr<Expr> expr, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Expr : public Node {
	public:

		Expr(Location);
		Expr(Type, Location);

		virtual void accept(Visitor &) = 0;
	};

	/* Declaration */

	class Function : public Node {
	public:
		string id;
		Type signature;
        vector<unique_ptr<Case>> cases;
		Scope* scope;
		bool is_anon = false;

		Function(unique_ptr<Expr>);
		Function(string, Type, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Case : public Node {
	public:
		unique_ptr<Expr> expr;
		vector<unique_ptr<Pattern>> patterns;
		bool tail_rec = false;

		Case(unique_ptr<Expr>, vector<unique_ptr<Pattern>>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Call : public Expr {
	public:
		unique_ptr<Expr> callee;
		vector<unique_ptr<Expr>> exprs;

		Call(unique_ptr<Expr>, vector<unique_ptr<Expr>>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	/* Other Expressions */

	class List : public Expr {
	public:
		vector<unique_ptr<Expr>> exprs;

		List(vector<unique_ptr<Expr>>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Tuple : public Expr {
	public:
		vector<unique_ptr<Expr>> exprs;

		Tuple(vector<unique_ptr<Expr>>, Location);

		virtual void accept(Visitor &);

		string str();
	};
}



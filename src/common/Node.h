#pragma once
#include "Visitor.h"

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace common {
    enum TypeEnum {
        INT,
        FLOAT,
        BOOL,
        CHAR,
		STRING,
		LIST,
		TUPLE,
		SIGNATURE,
		EMPTYLIST,
		UNKNOWN
	};

	class Scope;

	/* Abstract Nodes */

	class Node {
	public:
		Type* node_type;

        // for the interpreter to save the children values of a Node
        // this should probably be moved into the visitor itself later
        Expr *val;

		virtual void accept(Visitor &v) = 0;
	};

	class Expr : public Node {
	public:
		virtual void accept(Visitor &v) = 0;
	};

	class BinaryOp : public Expr {
	public:
		Expr *left;
		Expr *right;

		virtual void accept(Visitor &v) = 0;
	};

	class UnaryOp : public Expr {
	public:
		Expr *child;

		virtual void accept(Visitor &v) = 0;
	};

	class Pattern : public Expr {
	public:
		virtual void accept(Visitor &v) = 0;
	};

	/* Declaration */

	class Program : public Node {
	public:
		std::vector<Function*> funcs;

		virtual void accept(Visitor &v);
	};

	class Function : public Node {
	public:
		std::string id;
		std::vector<Type*> types;
		std::vector<Case*> cases;

		Function() { }
		Function(std::string s) { id = s; }

		virtual void accept(Visitor &v);
	};

	class Case : public Node {
	public:
		std::vector<Pattern *> patterns;
		Expr *expr;

        // used by the profiler to determine growth for case
        std::vector<int> time_units;

		Case() { }
		Case(Expr *e) { expr = e; }

		virtual void accept(Visitor &v);
	};

	/* Binary Operators */

	class Or : public BinaryOp {
	public:

		Or() { }
		Or(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class And : public BinaryOp {
	public:

		And() { }
		And(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Equal : public BinaryOp {
	public:

		Equal() { }
		Equal(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class NotEqual : public BinaryOp {
	public:

		NotEqual() { }
		NotEqual(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Lesser : public BinaryOp {
	public:

		Lesser() { }
		Lesser(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Greater : public BinaryOp {
	public:

		Greater() { }
		Greater(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class LesserEq : public BinaryOp {
	public:

		LesserEq() { }
		LesserEq(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class GreaterEq : public BinaryOp {
	public:

		GreaterEq() { }
		GreaterEq(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Add : public BinaryOp {
	public:

		Add() { }
		Add(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Sub : public BinaryOp {
	public:

		Sub() { }
		Sub(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Mul : public BinaryOp {
	public:

		Mul() { }
		Mul(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Div : public BinaryOp {
	public:

		Div() { }
		Div(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class Mod : public BinaryOp {
	public:

		Mod() { }
		Mod(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	class ListAdd : public BinaryOp {
	public:

		ListAdd() { }
		ListAdd(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	/* Unary Operators */

	class Par : public UnaryOp {
	public:

		Par() { }
		Par(Expr *c) { child = c; }

		virtual void accept(Visitor &v);
	};

	class Not : public UnaryOp {
	public:

		Not() { }
		Not(Expr *c) { child = c; }

		virtual void accept(Visitor &v);
	};

	/* Literals */

	class Int : public Pattern {
	public:
		int value;

		Int() { }
		Int(int v) { value = v; }

		virtual void accept(Visitor &v);
	};

	class Float : public Pattern {
	public:
		float value;

		Float() { }
		Float(float v) { value = v; }

		virtual void accept(Visitor &v);
	};

	class Bool : public Pattern {
	public:
		bool value;

		Bool() { }
		Bool(bool v) { value = v; }

		virtual void accept(Visitor &v);
	};

	class Char : public Pattern {
	public:
		char value;
		std::string tmp;

		Char() { }
		Char(char v) { value = v; }
		Char(std::string v) { tmp = v; }

		virtual void accept(Visitor &v);
	};

	class String : public Pattern {
	public:
		std::string value;

		String() { }
		String(std::string v) { value = v; }

		virtual void accept(Visitor &v);
	};

	class ListPattern : public Pattern {
	public:
		std::vector<Pattern *> patterns;

		virtual void accept(Visitor &v);
	};

	class TuplePattern : public Pattern {
	public:
		std::vector<Pattern *> patterns;

		virtual void accept(Visitor &v);
	};

	class ListSplit : public Pattern {
	public:
		Pattern *left;
		Pattern *right;

		ListSplit() { }
		ListSplit(Pattern *l, Pattern *r) { left = l; right = r; }

		virtual void accept(Visitor &v);
	};

	/* Other Expressions */

	class List : public Expr {
	public:
		std::vector<Expr*> exprs;

		virtual void accept(Visitor &v);
	};

	class Tuple : public Expr {
	public:
		std::vector<Expr*> exprs;

		virtual void accept(Visitor &v);
	};

	class Id : public Pattern {
	public:
		std::string id;
		Scope *scope;

		Id() { }
		Id(std::string i) { id = i; }

		virtual void accept(Visitor &v);
	};

	class Call : public Expr {
	public:
		Expr *callee;
		std::vector<Expr*> exprs;

        //indicates if the call should be parallelized
        bool par = false;

		Call() { }
		Call(Expr *c) { callee = c; }

		virtual void accept(Visitor &v);
	};

	class Type : public Node {
	public:
		TypeEnum type = UNKNOWN;
		std::vector<Type *> types;

        Type() { }
		Type(TypeEnum t) { type = t; }
		Type(TypeEnum t, std::vector<Type *> *ts) { type = t; types = *ts; }

		virtual void accept(Visitor &v);
		bool operator==(const Type &other) const;
	};
}

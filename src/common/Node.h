#pragma once
#include "Visitor.h"

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace common {
    enum class Types {
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

		int line_no;

		Node();
		Node(int);

		virtual void accept(Visitor &) = 0;
	};

	class Expr : public Node {
	public:

		Expr();
		Expr(int);

		virtual void accept(Visitor &) = 0;
	};

	class BinaryOp : public Expr {
	public:
		Expr *left;
		Expr *right;

		BinaryOp();
		BinaryOp(Expr *, Expr *);
		BinaryOp(Expr *, Expr *, int);

		virtual void accept(Visitor &) = 0;
	};

	class UnaryOp : public Expr {
	public:
		Expr *child;

		UnaryOp();
		UnaryOp(Expr *);
		UnaryOp(Expr *, int);

		virtual void accept(Visitor &) = 0;
	};

	class Pattern : public Expr {
	public:

		Pattern();
		Pattern(int);

		virtual void accept(Visitor &) = 0;
	};

	/* Declaration */

	class Program : public Node {
	public:
		vector<Function*> funcs;

		Program();
		Program(vector<Function*>);
		Program(vector<Function*>, int);

		virtual void accept(Visitor &);
	};

	class Function : public Node {
	public:
		string id;
		vector<Type*> types;
        vector<Case*> cases;
		Scope *scope;

		Function();
		Function(std::string);
		Function(string, vector<Type*>);
		Function(string, vector<Type*>, vector<Case*>);
		Function(string, vector<Type*>, vector<Case*>, int);

		virtual void accept(Visitor &);
	};

	class Case : public Node {
	public:
		vector<Pattern *> patterns;
		Expr *expr;

        // used by the profiler to determine growth for case
		// the first element in the vector is for the first test
		// second is for the second test, and so on
        std::vector<int> time_units;

		// used by the profiler to check the degree of dependencies
		// a dependency of 0 means that it doesn't depend on other function
		// 1 means that it depends on itself or a function with a dependency of 0
		// 2 means that it depends on a function with a dependency of 1, and so on
		int dep_deg = 0;

		// used by the optimizer to indicate if a case is tail recursive
        bool tail_rec = false;

		Case();
		Case(Expr *);
		Case(Expr *, vector<Pattern *>);
		Case(Expr *, vector<Pattern *>, int);

		virtual void accept(Visitor &);
	};

	/* Binary Operators */

	class Or : public BinaryOp {
	public:

		Or();
		Or(Expr *, Expr *);
		Or(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class And : public BinaryOp {
	public:

		And();
		And(Expr *, Expr *);
		And(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Equal : public BinaryOp {
	public:

		Equal();
		Equal(Expr *, Expr *);
		Equal(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class NotEqual : public BinaryOp {
	public:

		NotEqual();
		NotEqual(Expr *, Expr *);
		NotEqual(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Lesser : public BinaryOp {
	public:

		Lesser();
		Lesser(Expr *, Expr *);
		Lesser(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Greater : public BinaryOp {
	public:

		Greater();
		Greater(Expr *, Expr *);
		Greater(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class LesserEq : public BinaryOp {
	public:

		LesserEq();
		LesserEq(Expr *, Expr *);
		LesserEq(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class GreaterEq : public BinaryOp {
	public:

		GreaterEq();
		GreaterEq(Expr *, Expr *);
		GreaterEq(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Add : public BinaryOp {
	public:

		Add();
		Add(Expr *, Expr *);
		Add(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Sub : public BinaryOp {
	public:

		Sub();
		Sub(Expr *, Expr *);
		Sub(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Mul : public BinaryOp {
	public:

		Mul();
		Mul(Expr *, Expr *);
		Mul(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Div : public BinaryOp {
	public:

		Div();
		Div(Expr *, Expr *);
		Div(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class Mod : public BinaryOp {
	public:

		Mod();
		Mod(Expr *, Expr *);
		Mod(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	class ListAdd : public BinaryOp {
	public:

		ListAdd();
		ListAdd(Expr *, Expr *);
		ListAdd(Expr *, Expr *, int);

		virtual void accept(Visitor &);
	};

	/* Unary Operators */

	class Par : public UnaryOp {
	public:

		Par();
		Par(Expr *);
		Par(Expr *, int);

		virtual void accept(Visitor &);
	};

	class Not : public UnaryOp {
	public:

		Not();
		Not(Expr *);
		Not(Expr *, int);

		virtual void accept(Visitor &);
	};

	/* Literals */

	class Int : public Pattern {
	public:
		long value;

		Int(long);
		Int(long, int);

		virtual void accept(Visitor &);
	};

	class Float : public Pattern {
	public:
		double value;

		Float(double);
		Float(double, int);

		virtual void accept(Visitor &);
	};

	class Bool : public Pattern {
	public:
		bool value;

		Bool(bool);
		Bool(bool, int);

		virtual void accept(Visitor &);
	};

	class Char : public Pattern {
	public:
		char value;

		Char(char);
		Char(char, int);

		virtual void accept(Visitor &);
	};

	class String : public Pattern {
	public:
		string value;

		String(std::string);
		String(string, int);

		virtual void accept(Visitor &);
	};

	class ListPattern : public Pattern {
	public:
		vector<Pattern *> patterns;

		ListPattern();
		ListPattern(vector<Pattern *>);
		ListPattern(vector<Pattern *>, int);

		virtual void accept(Visitor &);
	};

	class TuplePattern : public Pattern {
	public:
		vector<Pattern *> patterns;

		TuplePattern();
		TuplePattern(vector<Pattern *>);
		TuplePattern(vector<Pattern *>, int);

		virtual void accept(Visitor &);
	};

	class ListSplit : public Pattern {
	public:
		Pattern *left;
		Pattern *right;

		ListSplit();
		ListSplit(Pattern *, Pattern *);
		ListSplit(Pattern *, Pattern *, int);

		virtual void accept(Visitor &);
	};

	/* Other Expressions */

	class List : public Expr {
	public:
		vector<Expr*> exprs;

		List();
		List(vector<Expr*>);
		List(vector<Expr*>, int);

		virtual void accept(Visitor &);
	};

	class Tuple : public Expr {
	public:
		vector<Expr*> exprs;

		Tuple();
		Tuple(vector<Expr*>);
		Tuple(vector<Expr*>, int);

		virtual void accept(Visitor &);
	};

	class Id : public Pattern {
	public:
		string id;
		Scope *scope;

		Id();
		Id(std::string);
		Id(string, int);

		virtual void accept(Visitor &);
	};

	class Call : public Expr {
	public:
		Expr *callee;
		vector<Expr*> exprs;

        //indicates if the call should be parallelized
        bool par = false;

		Call();
		Call(Expr *);
		Call(Expr *, vector<Expr*>);
		Call(Expr *, vector<Expr*>, int);

		virtual void accept(Visitor &);
	};

	class Type : public Node {
	public:
		Types type;
		vector<Type *> types;

		Type();
		Type(Types);
		Type(Types, vector<Type *>);
		Type(Types, vector<Type *>, int);
		virtual ~Type();

		virtual void accept(Visitor &v);
		bool operator==(const Type &other) const;

	};
}

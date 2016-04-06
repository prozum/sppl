#pragma once

#include "Location.h"

#include <vector>
#include <string>
#include <memory>

using namespace std;

#define ANON_FUNC_NAME "__anon_func"

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


	string collection_str(Type &node, const std::string split);

	// Abstract Nodes
	class Node {
	public:
		Type* node_type = nullptr;

        // for the interpreter to save the children values of a Node
        // this should probably be moved into the visitor itself later
        Expr *val = nullptr;

		Location loc;

		Node();
		Node(Location);

		virtual string str() = 0;
		virtual void accept(Visitor &) = 0;
	};

	class Type : public Node {
	public:
		Types type;
		vector<Type *> types;

		Type();
		Type(Types);
		Type(Types, Location);
		Type(Types, vector<Type *>);
		Type(Types, vector<Type *>, Location);
		virtual ~Type();

		virtual void accept(Visitor &v);
		bool operator==(const Type &other) const;
		bool operator!=(const Type &other) const;

		string str();
	};

	class Expr : public Node {
	public:

		Expr();
		Expr(Location);

		virtual void accept(Visitor &) = 0;
	};

	class BinaryOp : public Expr {
	public:
		Expr *left = nullptr;
		Expr *right = nullptr;

		BinaryOp();
		BinaryOp(Expr *, Expr *);
		BinaryOp(Expr *, Expr *, Location);

		virtual void accept(Visitor &) = 0;
	};

	class UnaryOp : public Expr {
	public:
		Expr *child = nullptr;

		UnaryOp();
		UnaryOp(Expr *);
		UnaryOp(Expr *, Location);

		virtual void accept(Visitor &) = 0;
	};

	class Pattern : public Expr {
	public:

		Pattern();
		Pattern(Location);

		virtual void accept(Visitor &) = 0;
	};

	/* Declaration */

	class Program : public Node {
	public:

		vector<Function*> funcs;

		Program();
		Program(vector<Function*>);
		Program(vector<Function*>, Location);
		Program(string id, Expr *expr, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Function : public Node {
	public:
		string id;
		vector<Type*> types;
        vector<Case*> cases;
		Scope *scope = nullptr;

		Function();
		Function(std::string);
		Function(string, vector<Type*>);
		Function(string, vector<Type*>, Location);
		Function(string, vector<Type*>, vector<Case*>);
		Function(string, vector<Type*>, vector<Case*>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Case : public Node {
	public:
		Expr *expr = nullptr;
		vector<Pattern *> patterns;

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
		Case(Expr *, vector<Pattern *>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	/* Binary Operators */

	class Or : public BinaryOp {
	public:

		Or();
		Or(Expr *, Expr *);
		Or(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class And : public BinaryOp {
	public:

		And();
		And(Expr *, Expr *);
		And(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Equal : public BinaryOp {
	public:

		Equal();
		Equal(Expr *, Expr *);
		Equal(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class NotEqual : public BinaryOp {
	public:

		NotEqual();
		NotEqual(Expr *, Expr *);
		NotEqual(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Lesser : public BinaryOp {
	public:

		Lesser();
		Lesser(Expr *, Expr *);
		Lesser(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Greater : public BinaryOp {
	public:

		Greater();
		Greater(Expr *, Expr *);
		Greater(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class LesserEq : public BinaryOp {
	public:

		LesserEq();
		LesserEq(Expr *, Expr *);
		LesserEq(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class GreaterEq : public BinaryOp {
	public:

		GreaterEq();
		GreaterEq(Expr *, Expr *);
		GreaterEq(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Add : public BinaryOp {
	public:

		Add();
		Add(Expr *, Expr *);
		Add(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Sub : public BinaryOp {
	public:

		Sub();
		Sub(Expr *, Expr *);
		Sub(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Mul : public BinaryOp {
	public:

		Mul();
		Mul(Expr *, Expr *);
		Mul(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Div : public BinaryOp {
	public:

		Div();
		Div(Expr *, Expr *);
		Div(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Mod : public BinaryOp {
	public:

		Mod();
		Mod(Expr *, Expr *);
		Mod(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class ListAdd : public BinaryOp {
	public:

		ListAdd();
		ListAdd(Expr *, Expr *);
		ListAdd(Expr *, Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	/* Unary Operators */

	class Par : public UnaryOp {
	public:

		Par();
		Par(Expr *);
		Par(Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Not : public UnaryOp {
	public:

		Not();
		Not(Expr *);
		Not(Expr *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	/* Literals */

	class Int : public Pattern {
	public:
		long value;

		Int(long);
		Int(long, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Float : public Pattern {
	public:
		double value;

		Float(double);
		Float(double, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Bool : public Pattern {
	public:
		bool value;

		Bool(bool);
		Bool(bool, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Char : public Pattern {
	public:
		char value;

		Char(char);
		Char(char, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class String : public Pattern {
	public:
		string value;

		String(std::string);
		String(string, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class ListPattern : public Pattern {
	public:
		vector<Pattern *> patterns;

		ListPattern();
		ListPattern(vector<Pattern *>);
		ListPattern(vector<Pattern *>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class TuplePattern : public Pattern {
	public:
		vector<Pattern *> patterns;

		TuplePattern();
		TuplePattern(vector<Pattern *>);
		TuplePattern(vector<Pattern *>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class ListSplit : public Pattern {
	public:
		Pattern *left = nullptr;
		Pattern *right = nullptr;

		ListSplit();
		ListSplit(Pattern *, Pattern *);
		ListSplit(Pattern *, Pattern *, Location);

		virtual void accept(Visitor &);

		string str();
	};

	/* Other Expressions */

	class List : public Expr {
	public:
		vector<Expr*> exprs;

		List();
		List(vector<Expr*>);
		List(vector<Expr*>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Tuple : public Expr {
	public:
		vector<Expr*> exprs;

		Tuple();
		Tuple(vector<Expr*>);
		Tuple(vector<Expr*>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Id : public Pattern {
	public:
		string id;
		Scope *scope = nullptr;

		Id();
		Id(std::string);
		Id(string, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Call : public Expr {
	public:
		Expr *callee = nullptr;
		vector<Expr*> exprs;

        //indicates if the call should be parallelized
        bool par = false;

		Call();
		Call(Expr *);
		Call(Expr *, vector<Expr*>);
		Call(Expr *, vector<Expr*>, Location);

		virtual void accept(Visitor &);

		string str();
	};
}

namespace std
{
	template <>
    struct hash<common::Type>
    {
        std::size_t operator()(const common::Type& k) const
        {
            size_t res = std::hash<int>()(static_cast<int>(k.type));

            for (auto type : k.types) {
                res ^= (hash<common::Type>()(*type) << 1);
            }

            return res;
        }
    };
}

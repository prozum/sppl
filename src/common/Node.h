#ifndef AST_HEADER
#define AST_HEADER
#include <vector>
#include <string>

namespace common {
	class Visitor;
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
		virtual void visit(Program *node) = 0;
		virtual void visit(Function *node) = 0;
		virtual void visit(Case *node) = 0;

		virtual void visit(Or *node) = 0;
		virtual void visit(And *node) = 0;
		virtual void visit(Equal *node) = 0;
		virtual void visit(NotEqual *node) = 0;
		virtual void visit(Lesser *node) = 0;
		virtual void visit(Greater *node) = 0;
		virtual void visit(LesserEq *node) = 0;
		virtual void visit(GreaterEq *node) = 0;
		virtual void visit(Add *node) = 0;
		virtual void visit(Sub *node) = 0;
		virtual void visit(Mul *node) = 0;
		virtual void visit(Div *node) = 0;
		virtual void visit(Mod *node) = 0;
		virtual void visit(ListAdd *node) = 0;
		virtual void visit(Par *node) = 0;
		virtual void visit(Not *node) = 0;

		virtual void visit(Int *node) = 0;
		virtual void visit(Float *node) = 0;
		virtual void visit(Bool *node) = 0;
		virtual void visit(Char *node) = 0;
		virtual void visit(String *node) = 0;
		virtual void visit(ListPattern *node) = 0;
		virtual void visit(TuplePattern *node) = 0;
		virtual void visit(ListSplit *node) = 0;
		virtual void visit(List *node) = 0;
		virtual void visit(Tuple *node) = 0;
		virtual void visit(Id *node) = 0;
		virtual void visit(Call *node) = 0;

		virtual void visit(IntType *node) = 0;
		virtual void visit(FloatType *node) = 0;
		virtual void visit(BoolType *node) = 0;
		virtual void visit(CharType *node) = 0;
		virtual void visit(StringType *node) = 0;
		virtual void visit(ListType *node) = 0;
		virtual void visit(TupleType *node) = 0;
		virtual void visit(Signature *node) = 0;
	};

	/* Abstract Nodes */

	class Node {
	public:
		virtual void accept(Visitor *v) = 0;
	};

	class Expr : public Node {
	public:
		virtual void accept(Visitor *v) = 0;
	};

	class BinaryOp : public Expr {
	public:
		Expr *left;
		Expr *right;

		virtual void accept(Visitor *v) = 0;
	};

	class UnaryOp : public Expr {
	public:
		Expr *child;

		virtual void accept(Visitor *v) = 0;
	};

	class Type : public Node {
	public:
		virtual void accept(Visitor *v) = 0;
	};

	class Pattern : public Expr {
	public:
		virtual void accept(Visitor *v) = 0;
	};

	/* Declaration */

	class Program : public Node {
	public:
		std::vector<Function*> funcs;

		virtual void accept(Visitor *v);
	};

	class Function : public Node {
	public:
		std::string id;
		std::vector<Type*> types;
		std::vector<Case*> cases;

		Function() { }
		Function(std::string s) { id = s; }

		virtual void accept(Visitor *v);
	};

	class Case : public Node {
	public:
		std::vector<Pattern *> patterns;
		Expr *expr;

		Case() { }
		Case(Expr *e) { expr = e; }

		virtual void accept(Visitor *v);
	};

	/* Binary Operators */

	class Or : public BinaryOp {
	public:

		Or() { }
		Or(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class And : public BinaryOp {
	public:

		And() { }
		And(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Equal : public BinaryOp {
	public:

		Equal() { }
		Equal(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class NotEqual : public BinaryOp {
	public:

		NotEqual() { }
		NotEqual(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Lesser : public BinaryOp {
	public:

		Lesser() { }
		Lesser(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Greater : public BinaryOp {
	public:

		Greater() { }
		Greater(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class LesserEq : public BinaryOp {
	public:

		LesserEq() { }
		LesserEq(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class GreaterEq : public BinaryOp {
	public:

		GreaterEq() { }
		GreaterEq(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Add : public BinaryOp {
	public:

		Add() { }
		Add(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Sub : public BinaryOp {
	public:

		Sub() { }
		Sub(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Mul : public BinaryOp {
	public:

		Mul() { }
		Mul(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Div : public BinaryOp {
	public:

		Div() { }
		Div(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class Mod : public BinaryOp {
	public:

		Mod() { }
		Mod(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	class ListAdd : public BinaryOp {
	public:

		ListAdd() { }
		ListAdd(Expr *l, Expr *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	/* Unary Operators */

	class Par : public UnaryOp {
	public:

		Par() { }
		Par(Expr *c) { child = c; }

		virtual void accept(Visitor *v);
	};

	class Not : public UnaryOp {
	public:

		Not() { }
		Not(Expr *c) { child = c; }

		virtual void accept(Visitor *v);
	};

	/* Literals */

	class Int : public Pattern {
	public:
		int value;

		Int() { }
		Int(int v) { value = v; }

		virtual void accept(Visitor *v);
	};

	class Float : public Pattern {
	public:
		float value;

		Float() { }
		Float(float v) { value = v; }

		virtual void accept(Visitor *v);
	};

	class Bool : public Pattern {
	public:
		bool value;

		Bool() { }
		Bool(bool v) { value = v; }

		virtual void accept(Visitor *v);
	};

	class Char : public Pattern {
	public:
		char value;
		std::string tmp;

		Char() { }
		Char(char v) { value = v; }
		Char(std::string v) { tmp = v; }

		virtual void accept(Visitor *v);
	};

	class String : public Pattern {
	public:
		std::string value;

		String() { }
		String(std::string v) { value = v; }

		virtual void accept(Visitor *v);
	};

	class ListPattern : public Pattern {
	public:
		std::vector<Pattern *> patterns;

		virtual void accept(Visitor *v);
	};

	class TuplePattern : public Pattern {
	public:
		std::vector<Pattern *> patterns;

		virtual void accept(Visitor *v);
	};

	class ListSplit : public Pattern {
	public:
		Pattern *left;
		Pattern *right;

		ListSplit() { }
		ListSplit(Pattern *l, Pattern *r) { left = l; right = r; }

		virtual void accept(Visitor *v);
	};

	/* Other Expressions */

	class List : public Expr {
	public:
		std::vector<Expr*> exprs;

		virtual void accept(Visitor *v);
	};

	class Tuple : public Expr {
	public:
		std::vector<Expr*> exprs;

		virtual void accept(Visitor *v);
	};

	class Id : public Pattern {
	public:
		std::string id;

		Id() { }
		Id(std::string i) { id = i; }

		virtual void accept(Visitor *v);
	};

	class Call : public Expr {
	public:
		Expr *callee;
		std::vector<Expr*> exprs;

		Call() { }
		Call(Expr *c) { callee = c; }

		virtual void accept(Visitor *v);
	};

	/* Types */

	class IntType : public Type {
	public:
		virtual void accept(Visitor *v);
	};

	class FloatType : public Type {
	public:
		virtual void accept(Visitor *v);
	};

	class BoolType : public Type {
	public:
		virtual void accept(Visitor *v);
	};

	class CharType : public Type {
	public:
		virtual void accept(Visitor *v);
	};

	class StringType : public Type {
	public:
		virtual void accept(Visitor *v);
	};

	class ListType : public Type {
	public:
		Type *type;

		ListType() { }
		ListType(Type *t) { type = t; }

		virtual void accept(Visitor *v);
	};

	class TupleType : public Type {
	public:
		std::vector<Type*> types;

		virtual void accept(Visitor *v);
	};

	class Signature : public Type {
	public:
		std::vector<Type*> types;

		virtual void accept(Visitor *v);
	};
}

#endif

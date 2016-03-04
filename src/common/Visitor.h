#pragma once

namespace common {

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
	class LiteralType;
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

		virtual void visit(LiteralType *node) = 0;

		virtual void visit(ListType *node) = 0;

		virtual void visit(TupleType *node) = 0;

		virtual void visit(Signature *node) = 0;
	};
}
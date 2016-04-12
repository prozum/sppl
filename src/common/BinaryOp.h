#pragma once

#include "Node.h"

namespace common {

	class BinaryOp : public Expr {
	public:
		unique_ptr<Expr> left;
		unique_ptr<Expr> right;

		BinaryOp(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &) = 0;
	};

	class Add : public BinaryOp {
	public:

		Add(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Sub : public BinaryOp {
	public:

		Sub(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Mul : public BinaryOp {
	public:

		Mul(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Div : public BinaryOp {
	public:

		Div(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Mod : public BinaryOp {
	public:

		Mod(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class ListAdd : public BinaryOp {
	public:

		ListAdd(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

		class Or : public BinaryOp {
	public:

		Or(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class And : public BinaryOp {
	public:

		And(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Equal : public BinaryOp {
	public:

		Equal(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class NotEqual : public BinaryOp {
	public:

		NotEqual(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Lesser : public BinaryOp {
	public:

		Lesser(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Greater : public BinaryOp {
	public:

		Greater(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class LesserEq : public BinaryOp {
	public:

		LesserEq(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class GreaterEq : public BinaryOp {
	public:

		GreaterEq(unique_ptr<Expr>, unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};
}

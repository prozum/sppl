#pragma once

#include "Expression.h"
#include <memory>

namespace common {

	class UnaryOp : public Expression {
	public:
		unique_ptr<Expression> Child;

		UnaryOp(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V) = 0;
	};

	class ParExpr : public UnaryOp {
	public:

		ParExpr(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class Not : public UnaryOp {
	public:

		Not(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class Negative : public UnaryOp {
	public:

		Negative(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class To : public UnaryOp {
	public:
		Type TypeCast;

		To(unique_ptr<Expression> Child, Type TypeCast, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	template<class T>
	unique_ptr<Node> cloneUnaryOp(UnaryOp& Op);
}
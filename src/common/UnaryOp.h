#pragma once

#include "Node.h"

#include <memory>

namespace common {

	class UnaryOp : public Expression {
	public:
		unique_ptr<Expression> Child;

		UnaryOp(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V) = 0;
	};

	class Par : public UnaryOp {
	public:

		Par(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Not : public UnaryOp {
	public:

		Not(unique_ptr<Expression> Child, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};
}
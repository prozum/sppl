#pragma once

#include "Node.h"

#include <memory>

namespace common {

	class UnaryOp : public Expr {
	public:
		unique_ptr<Expr> child;

		UnaryOp(unique_ptr<Expr>, Location);

		virtual void accept(Visitor &) = 0;
	};

	class Par : public UnaryOp {
	public:

		Par(unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Not : public UnaryOp {
	public:

		Not(unique_ptr<Expr>, Location);

		virtual void accept(Visitor &);

		string str();
	};
}
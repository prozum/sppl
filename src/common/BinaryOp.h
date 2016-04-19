#pragma once

#include "Node.h"
#include "Expression.h"

namespace common {

	class BinaryOp : public Expression {
	public:
		unique_ptr<Expression> Left;
		unique_ptr<Expression> Right;

		BinaryOp(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V) = 0;
	};

	class Add : public BinaryOp {
	public:
		Add(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Sub : public BinaryOp {
	public:
		Sub(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Mul : public BinaryOp {
	public:
		Mul(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Div : public BinaryOp {
	public:
		Div(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &v);
		string str();
	};

	class Mod : public BinaryOp {
	public:
		Mod(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class ListAdd : public BinaryOp {
	public:
		ListAdd(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

		class Or : public BinaryOp {
	public:
		Or(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class And : public BinaryOp {
	public:
		And(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Equal : public BinaryOp {
	public:
		Equal(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class NotEqual : public BinaryOp {
	public:
		NotEqual(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Lesser : public BinaryOp {
	public:
		Lesser(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Greater : public BinaryOp {
	public:
		Greater(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class LesserEq : public BinaryOp {
	public:
		LesserEq(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class GreaterEq : public BinaryOp {
	public:
		GreaterEq(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class ProducerConsumer : public BinaryOp {
	public:
		ProducerConsumer(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Concat : public BinaryOp {
	public:
		Concat(unique_ptr<Expression> Left, unique_ptr<Expression> Right, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};
}

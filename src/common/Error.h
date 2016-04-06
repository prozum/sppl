#pragma once

#include "Location.h"
#include "Node.h"

#include <cstddef>
#include <string>

namespace common {
	class Error {
	public:
		Error(string msg, Location loc);
		static Error Expected(string msg, string expected, string actual, Location loc)
		{
			return Error(msg + ": Expected: '" + expected + "' Actual: '" + actual + "'", loc);
		};

		static Error Binary(string msg, common::BinaryOp &op)
		{
			return Error("'" + op.str() + "' " + msg + ": Left: '" + op.left->str() + "' Right: '" + op.right->str() + "'", op.loc);
		};

		static Error Unary(string msg, common::UnaryOp &op)
		{
			return Error("'" + op.str() + "' " + msg + ": Child: '" + op.child->str() + "'", op.loc);
		};


		std::string msg;
		Location loc;
	};

	std::ostream &operator<<(ostream &out, const Error &err);
}

#pragma once

#include "Location.h"
#include "Common.h"

#include <cstddef>
#include <string>

namespace common {
	class Error {
	public:
		Error(string msg);
		Error(string msg, Location loc);

		static Error NotImplemented(string msg);
		static Error Expected(string msg, string expected, string actual, Location loc);
		static Error Binary(string msg, common::BinaryOp &op);
		static Error Unary(string msg, common::UnaryOp &op);

		std::string msg;
		Location loc;
	};

	std::ostream &operator<<(ostream &out, const Error &err);
}

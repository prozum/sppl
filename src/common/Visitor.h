#pragma once

#include "Common.h"
#include "Error.h"

#include <vector>
#include <string>

namespace common {
    class Visitor {
		friend class Node;
	public:
		std::vector<Error> Errors;
		void addError(Error error);
		bool hasError();

		virtual void visit(Program &node) { throw "Not implemented"; }
		virtual void visit(Function &node) { throw "Not implemented"; }
		virtual void visit(Case &node) { throw "Not implemented"; }
		virtual void visit(Or &node) { throw "Not implemented"; }
		virtual void visit(And &node) { throw "Not implemented"; }
		virtual void visit(Equal &node) { throw "Not implemented"; }
		virtual void visit(NotEqual &node) { throw "Not implemented"; }
		virtual void visit(Lesser &node) { throw "Not implemented"; }
		virtual void visit(Greater &node) { throw "Not implemented"; }
		virtual void visit(LesserEq &node) { throw "Not implemented"; }
		virtual void visit(GreaterEq &node) { throw "Not implemented"; }
		virtual void visit(Add &node) { throw "Not implemented"; }
		virtual void visit(Sub &node) { throw "Not implemented"; }
		virtual void visit(Mul &node) { throw "Not implemented"; }
		virtual void visit(Div &node) { throw "Not implemented"; }
		virtual void visit(Mod &node) { throw "Not implemented"; }
		virtual void visit(ListAdd &node) { throw "Not implemented"; }
		virtual void visit(Par &node) { throw "Not implemented"; }
		virtual void visit(Not &node) { throw "Not implemented"; }
		virtual void visit(Int &node) { throw "Not implemented"; }
		virtual void visit(Float &node) { throw "Not implemented"; }
		virtual void visit(Bool &node) { throw "Not implemented"; }
		virtual void visit(Char &node) { throw "Not implemented"; }
		virtual void visit(String &node) { throw "Not implemented"; }
		virtual void visit(ListPattern &node) { throw "Not implemented"; }
		virtual void visit(TuplePattern &node) { throw "Not implemented"; }
		virtual void visit(ListSplit &node) { throw "Not implemented"; }
		virtual void visit(List &node) { throw "Not implemented"; }
		virtual void visit(Tuple &node) { throw "Not implemented"; }
		virtual void visit(Id &node) { throw "Not implemented"; }
		virtual void visit(Call &node) { throw "Not implemented"; }
		//virtual void visit(TypeNode &node) { throw "Not implemented"; }
	};
}
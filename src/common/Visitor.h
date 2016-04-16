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

		virtual void visit(Program &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Function &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Case &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Or &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(And &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Equal &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(NotEqual &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Lesser &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Greater &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(LesserEq &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(GreaterEq &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Add &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Sub &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Mul &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Div &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Mod &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListAdd &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Par &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Not &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Int &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Float &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Bool &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Char &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(String &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListPattern &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(TuplePattern &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListSplit &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(List &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Tuple &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Id &node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Call &node) { throw runtime_error( "Not implemented"); }
	};
}
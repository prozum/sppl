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
		void addError(Error Err);
		bool hasError();

		virtual void visit(Program &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Function &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Case &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Or &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(And &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Equal &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(NotEqual &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Lesser &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Greater &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(LesserEq &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(GreaterEq &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Add &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Sub &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Mul &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Div &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Mod &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListAdd &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ProducerConsumer &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Concat &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(To &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Par &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Not &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Negative &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(LambdaFunction &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(IdPattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(IntPattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(FloatPattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(CharPattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListPattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(TuplePattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListSplit &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(WildPattern &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(IdExpression &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(IntExpression &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(FloatExpression &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(CharExpression &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(ListExpression &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(TupleExpression &Node) { throw runtime_error( "Not implemented"); }
		virtual void visit(Call &Node) { throw runtime_error( "Not implemented"); }
	};
}
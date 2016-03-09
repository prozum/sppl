#pragma once

#include <ostream>
#include <string>
#include "Visitor.h"
#include "Node.h"

using namespace common;

namespace visitor {

    class Optimizer : public common::Visitor {

        void visit(Program *node);

        void visit(Function *node);

        void visit(Case *node);

        void visit(Or *node);

        void visit(And *node);

        void visit(Equal *node);

        void visit(NotEqual *node);

        void visit(Lesser *node);

        void visit(Greater *node);

        void visit(LesserEq *node);

        void visit(GreaterEq *node);

        void visit(Add *node);

        void visit(Sub *node);

        void visit(Mul *node);

        void visit(Div *node);

        void visit(Mod *node);

        void visit(ListAdd *node);

        void visit(Par *node);

        void visit(Not *node);

        void visit(Int *node);

        void visit(Float *node);

        void visit(Bool *node);

        void visit(Char *node);

        void visit(String *node);

        void visit(ListPattern *node);

        void visit(TuplePattern *node);

        void visit(ListSplit *node);

        void visit(List *node);

        void visit(Tuple *node);

        void visit(Id *node);

        void visit(Call *node);

        void visit(Type *node);
    };

}

#pragma once

#include "CodeGenerator.h"
#include <string>

using namespace common;

namespace codegen {
    class HCodeGenerator: public parser::CodeGenerator
    {
        public:
            HCodeGenerator(parser::Driver &Drv);

            void visit(Program &Node);
            void visit(Function &Node);
            void visit(Case &Node);
            void visit(Or &Node);
            void visit(And &Node);
            void visit(Equal &Node);
            void visit(NotEqual &Node);
            void visit(Lesser &Node);
            void visit(Greater &Node);
            void visit(LesserEq &Node);
            void visit(GreaterEq &Node);
            void visit(Add &Node);
            void visit(Sub &Node);
            void visit(Mul &Node);
            void visit(Div &Node);
            void visit(Mod &Node);
            void visit(ListAdd &Node);
            void visit(Par &Node);
            void visit(Not &Node);
            void visit(IntPattern &Node);
            void visit(FloatPattern &Node);
            void visit(BoolPattern &Node);
            void visit(CharPattern &Node);
            void visit(ListPattern &Node);
            void visit(TuplePattern &Node);
            void visit(ListSplit &Node);
            void visit(IdPattern &Node);
            void visit(Call &Node);

        private:
            Function *CurFunc;
    };
}

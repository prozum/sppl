#pragma once

#include "Node.h"
#include "Visitor.h"

namespace semantics {

    class CaseChecker : public common::Visitor {
    public:
        string currentFunction;
        int expectedPatternCount;
        bool hasDefault = false;
        bool hasError = false;
        bool isId = false;

        void visit(common::Program &node);
        void visit(common::Function &node);
        void visit(common::Case &node);

        void visit(common::Int &node);
        void visit(common::Float &node);
        void visit(common::Char &node);
        void visit(common::String &node);
        void visit(common::Bool &node);
        void visit(common::Id &node);

    private:
    };

}
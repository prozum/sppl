#pragma once
#include "CppCodeGenerator.h"

namespace codegen {
class CParCodeGenerator : public CCodeGenerator {
    public:
        CParCodeGenerator(parser::Driver &Drv);
        void visit(common::Program &Node);

    private:
        void visit(common::Function &Node);
        void visit(common::Case &Node);
        void visit(common::CallExpr &Node);

        std::string generateEnvironment(common::Type &Ty);

        const std::string GTask = "task";
        const std::string GRes = "res";

        std::string CurrentArg;

        std::vector<std::string> CallStack;
        std::vector<std::string> CurrentTasks;

        size_t CallDepth = 0;
        int TaskCount = 0;
};
}

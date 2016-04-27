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
        const std::string GFuncPointer = "task_func";

        std::string CurrentArg;

        int TaskCount = 0;
        int SupTaskCount = 0;
};
}

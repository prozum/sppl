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
        const std::string GParallel = "par_";
        const std::string GSequential = "seq_";

        std::string CurrentArg;
        std::string SequentialCall;

        std::vector<std::string> CallStack;
        std::vector<int> CallStackCount;
        std::vector<std::string> CurrentTasks;

        bool GenerateParallel;

        size_t CallDepth = 0;
        int TaskCount = 0;
};
}

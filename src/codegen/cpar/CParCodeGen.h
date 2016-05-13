
#pragma once
#include "CCodeGen.h"

namespace codegen {
class CParCodeGen : public CCodeGen {
    public:
        CParCodeGen(parser::Driver &Drv);
        void visit(common::Program &Node);

    private:
        void visit(common::Function &Node);
        void visit(common::Case &Node);
        void visit(common::CallExpr &Node);

        std::string generateEnvironment(common::Type &Ty);

        const std::string GTask = "task";
        const std::string GRes = "res";
        const std::string GParallel = "par";
        const std::string GSequential = "seq";

        std::string CurrentArg;

        std::vector<std::string> TaskDeallocs;

        std::vector<std::string> SequentialCall;
        std::vector<size_t> CallStackCount;
        std::vector<std::string> CurrentTasks;

        bool GenerateParallel;

        size_t TaskCount = 0;

        void outputBeforeExpr();
};
}

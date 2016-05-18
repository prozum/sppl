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

    const std::string GPar = "par";
    const std::string GSeq = "call";
    const std::string GTask = GGenerated + "task";

    unsigned int TaskCount = 0;

    std::string CurrentArg;
    bool GenerateParallel;

    unsigned int CallDepth;
    std::vector<std::vector<ctree::Statement*>> ParallelCalls;
    std::vector<ctree::Statement*> SequentialCall;

    std::string generateSignature(common::Type &Ty);
    std::string getArgType(common::Type &Ty);
    std::string getArgName(common::Type &Ty);
    void outputParallel();
    void generateStd();
};
}

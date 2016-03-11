#pragma once
#include "CodeGenerator.h"

using namespace common;

namespace codegen {

    class LLVMCodeGenerator : public CodeGenerator
    {
    public:
        LLVMCodeGenerator(std::ostream &);

    };
}

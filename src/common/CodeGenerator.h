#pragma once

#include <ostream>
#include <string>
#include <llvm/IR/Function.h>
#include "Visitor.h"
#include "Node.h"

using namespace std;

namespace common {

    class CodeGenerator : public Visitor {
    public:

        CodeGenerator(ostream &out);

        std::ostream &output;

        bool jit_mode = false;
    };

}

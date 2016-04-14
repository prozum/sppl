#pragma once

#include <string>
#include "CodeGenerator.h"
#include "Node.h"

using namespace std;
using namespace common;
using namespace parser;

namespace codegen {
    class Printer : public CodeGenerator {
    public:
        Printer(Driver &driver);

        virtual void visit(Program &node);
    };

}

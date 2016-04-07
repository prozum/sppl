#pragma once

#include <string>
#include "CodeGenerator.h"
#include "Node.h"
using namespace common;
using namespace std;

namespace codegen {
    class Printer : public CodeGenerator {
    public:
        Printer(ostream &out);

        virtual void visit(Program &node);
    };

}

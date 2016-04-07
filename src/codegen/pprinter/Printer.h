#pragma once

#include <string>
#include "CodeGenerator.h"
#include "Node.h"
using namespace common;
using namespace std;

namespace codegen {
    class Printer : public CodeGenerator {
    public:
        Printer(shared_ptr<ostream> out);

        virtual void visit(Program &node);
    };

}

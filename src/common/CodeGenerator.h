#pragma once

#include <ostream>
#include <string>
#include "Visitor.h"
#include "Node.h"

using namespace std;

namespace common {

    class CodeGenerator : public Visitor {
    public:

        CodeGenerator(shared_ptr<ostream> out);

        shared_ptr<ostream> output;
    };

}

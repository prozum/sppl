#pragma once

#include <ostream>
#include <string>
#include "Visitor.h"
#include "Node.h"

using namespace std;
using namespace common;

namespace visitor {

    class CodeGenerator : public common::Visitor {
    public:
        CodeGenerator(std::ostream &);

    private:
        std::ostream &ostream;
    };

}

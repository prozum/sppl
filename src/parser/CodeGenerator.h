#pragma once

#include "Visitor.h"
#include "Node.h"
#include "Driver.h"

#include <ostream>
#include <string>

using namespace std;

namespace parser {

    class CodeGenerator : public Visitor {
    public:

        CodeGenerator(Driver &driver);

        Driver &driver;
    };

}

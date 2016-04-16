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
        Driver &Drv;

        CodeGenerator(Driver &driver);
    };

}

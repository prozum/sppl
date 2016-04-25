#pragma once

#include "Driver.h"
#include "Node.h"
#include "Visitor.h"

#include <ostream>
#include <string>

using namespace std;

namespace parser {

class CodeGenerator : public Visitor {
  public:
    Driver &Drv;

    CodeGenerator(Driver &Drv);
};
}

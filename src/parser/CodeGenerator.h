#pragma once

#include "Driver.h"
#include "Node.h"
#include "Visitor.h"

#include <ostream>
#include <string>

namespace parser {
class CodeGenerator : public common::Visitor {
  public:
    Driver &Drv;

    CodeGenerator(Driver &Drv);
};
}

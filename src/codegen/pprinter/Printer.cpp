#include "Printer.h"
#include <iostream>

using namespace std;

namespace codegen {

    Printer::Printer(shared_ptr<ostream> out)
        : CodeGenerator(out) { }

    void Printer::visit(Program &node) {
        *output << node.str();
    }
}

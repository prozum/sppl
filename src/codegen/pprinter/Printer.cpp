#include "Printer.h"
#include <iostream>

using namespace std;

namespace codegen {

    Printer::Printer(Driver &driver)
        : CodeGenerator(driver) { }

    void Printer::visit(Program &node) {
        *driver.out << node.str();
    }
}

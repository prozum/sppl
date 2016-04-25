#include "Printer.h"
#include <iostream>

using namespace std;

namespace codegen {

Printer::Printer(Driver &Drv) : CodeGenerator(Drv) {}

void Printer::visit(Program &Node) { *Drv.Out << Node.str(); }
}

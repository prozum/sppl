#include "Printer.h"

using namespace std;
using namespace codegen;

Printer::Printer(Driver &Drv) : CodeGenerator(Drv) {}

void Printer::visit(Program &Node) { *Drv.Out << Node.str(); }

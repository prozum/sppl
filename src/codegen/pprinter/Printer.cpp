#include "Printer.h"

using namespace std;
using namespace codegen;
using namespace common;

Printer::Printer(parser::Driver &Drv) : CodeGenerator(Drv) {}

void Printer::visit(Program &Node) { *Drv.Out << Node.str(); }

#include "GasCodeGenerator.h"

using namespace common;
using namespace std;

GasCodeGenerator::GasCodeGenerator(std::ostream &os) : visitor::CodeGenerator::CodeGenerator(os), os(os)
{

}
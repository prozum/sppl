#pragma once

#include "Compiler.h"

using namespace compiler;

class CCompiler: public Compiler {
    static void compile(std::istream& input, std::ostream& output);
};

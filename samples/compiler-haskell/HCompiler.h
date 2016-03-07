#pragma once

#include "Compiler.h"

using namespace compiler;

class HCompiler: public Compiler {
    static void compile(std::istream& input, std::ostream& output);
};

#pragma once

#include "Compiler.h"

class CCompiler: public Compiler {
    static void compile(std::istream& input, std::ostream& output);
};

#pragma once

#include "Compiler.h"

class HCompiler: public Compiler {
    static void compile(std::istream& input, std::ostream& output);
};

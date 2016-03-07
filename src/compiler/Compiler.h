#pragma once
#include <iostream>

namespace compiler {
    class Compiler {
        Compiler();

    public:
        static void compile(std::istream &input, std::ostream &output);
    };

}
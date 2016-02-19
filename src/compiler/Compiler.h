#pragma once
#include <iostream>

class Compiler {
    Compiler();

public:
    static void compile(std::istream& input, std::ostream& output);
};


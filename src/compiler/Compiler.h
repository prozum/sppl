#pragma once
#include <iostream>
#include <memory>
#include "Driver.h"
#include "CodeGenerator.h"

using namespace common;

namespace compiler {
    class Compiler {
    public:
        Compiler(istream &in, ostream &out);

        std::istream &input;
        std::ostream &output;
        std::unique_ptr<CodeGenerator> generator;
        parser::Driver driver;

        void compile();

        template <class T>
        void set_codegenerator();
    };

}
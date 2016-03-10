#pragma once
#include <iostream>
#include <memory>
#include "Driver.h"
#include "CodeGenerator.h"

using namespace common;

namespace compiler {

    enum Backend
    {
        CPP,
        GNUASM,
        HASKELL,
        LLVM,
        PPRINTER
    };

    class Compiler {
    public:
        Compiler(istream *in, ostream *out);
        Compiler(istream *in, ostream *out, ostream *hout);

        std::istream* input;
        std::ostream* output;
        std::ostream* header_output;
        std::unique_ptr<CodeGenerator> generator;
        parser::Driver driver;

        void compile();

        void set_backend(Backend backend);
    };

}
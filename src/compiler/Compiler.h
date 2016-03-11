#pragma once

#include <iostream>
#include <memory>
#include "Driver.h"
#include "CodeGenerator.h"

#ifdef CCPP
#include "CppCodeGenerator.h"
#endif

#ifdef CGNUASM
#include "GasCodeGenerator.h"
#endif

#ifdef CHASKELL
#include "HCodeGenerator.h"
#endif

#ifdef CLLVM
#include "LLVMCodeGenerator.h"
#endif

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

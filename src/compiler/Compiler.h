#pragma once

#include "Driver.h"
#include "CodeGenerator.h"
#include "TypeChecker.h"
#include "ScopeGenerator.h"

#include <iostream>
#include <memory>

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

        shared_ptr<std::istream> input;
        shared_ptr<std::ostream> output;
        shared_ptr<std::ostream> header_output;
        std::unique_ptr<CodeGenerator> generator;
        semantics::ScopeGenerator scope_generator;
        semantics::TypeChecker type_checker;
        parser::Driver driver;

        int compile();
        Function * parse_anon_expr();

        void set_backend(Backend backend);
    };

}

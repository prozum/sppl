#pragma once

#include "Driver.h"
#include "CodeGenerator.h"
#include "TypeChecker.h"
#include "ScopeGenerator.h"

#include <iostream>
#include <memory>
#include <PatternChecker.h>

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
        Compiler(shared_ptr<istream> in, shared_ptr<ostream> out, shared_ptr<ostream> hout);

        parser::Driver driver;
        shared_ptr<istream> input;
        shared_ptr<ostream> output;
        shared_ptr<ostream> header_output;
        unique_ptr<CodeGenerator> generator;
        semantics::ScopeGenerator scope_generator;
        semantics::TypeChecker type_checker;
        semantics::PatternChecker pattern_checker;

        int compile();

        void set_backend(Backend backend);
    };

}

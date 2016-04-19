#pragma once

#include "Driver.h"
#include "CodeGenerator.h"
#include "TypeChecker.h"
#include "ScopeGenerator.h"
#include "GeneralOptimizer.h"

#include <iostream>
#include <memory>
#include <fstream>

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


using namespace parser;
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

    class Compiler : public Driver {
    public:
        Compiler();

        unique_ptr<CodeGenerator> generator;
        semantics::ScopeGenerator scope_generator;
        semantics::TypeChecker type_checker;
        optimizer::GeneralOptimizer optimizer;

        int compile();

        void set_backend(Backend B);
    };

}

#pragma once

#include "CodeGenerator.h"
#include "Driver.h"
#include "GeneralOptimizer.h"
#include "ScopeGenerator.h"
#include "TypeChecker.h"

#include <fstream>
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

using namespace parser;
using namespace common;

namespace compiler {

enum Backend { CPP, GNUASM, HASKELL, LLVM, PPRINTER };

class Compiler : public Driver {
  public:
    Compiler();

    unique_ptr<CodeGenerator> CodeGen;
    semantics::ScopeGenerator ScopeGen;
    semantics::TypeChecker TypeChecker;
    optimizer::GeneralOptimizer Optimizer;

    int compile(const string &Filename);
    int compile(const vector<string> &Filenames);

    void setBackend(Backend B);
};
}

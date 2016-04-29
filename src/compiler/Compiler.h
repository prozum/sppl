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
#include "CParCodeGenerator.h"
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

namespace compiler {

enum Backend { CPP, GNUASM, HASKELL, LLVM, PPRINTER, CPAR };

class Compiler : public parser::Driver {
  public:
    Compiler();

    std::unique_ptr<parser::CodeGenerator> CodeGen;
    semantics::ScopeGenerator ScopeGen;
    semantics::TypeChecker TypeChecker;
#if OPTIMIZER
    optimizer::GeneralOptimizer Optimizer;
#endif

    int compile(const std::string &Filename);
    int compile(const std::vector<std::string> &Filenames);

    void setBackend(Backend B);
};
}

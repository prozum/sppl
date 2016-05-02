#include "Compiler.h"

using namespace compiler;
using namespace std;

Compiler::Compiler()
    : ScopeGen(semantics::ScopeGenerator(&Global)),
      TypeChecker(semantics::TypeChecker(&Global)) {}

void Compiler::setBackend(Backend B) {
    switch (B) {
#ifdef CCPP
    case Backend::CPP:
        CodeGen = std::make_unique<codegen::CCodeGenerator>(*this);
        break;
    case Backend::CPAR:
        CodeGen = std::make_unique<codegen::CParCodeGenerator>(*this);
        break;
#endif
#ifdef CGNUASM
    case Backend::GNUASM:
        CodeGen = std::make_unique<codegen::GasCodeGenerator>(*this);
        break;
#endif
#ifdef CHASKELL
    case Backend::HASKELL:
        CodeGen = std::make_unique<codegen::HCodeGenerator>(*this);
        break;
#endif
#ifdef CLLVM
    case Backend::LLVM:
        CodeGen = std::make_unique<codegen::LLVMCodeGenerator>(*this);
        break;
#endif
    case Backend::PPRINTER:
        CodeGen = std::make_unique<codegen::Printer>(*this);
        break;
    default:
        cerr << "Not a valid backend!" << endl;
    }
}

int Compiler::compile(const string &Filename) {
    return compile(vector<string>({Filename}));
}

int Compiler::compile(const vector<string> &Filenames) {

    if (!parseFiles(Filenames))
        return 1;

    if (!accept(ScopeGen))
        return 2;

    if (!accept(TypeChecker))
        return 3;

#if OPTIMIZER
    if (!accept(Optimizer))
        return 4;
#endif

    if (!accept(*CodeGen))
        return 5;

    return 0;
}

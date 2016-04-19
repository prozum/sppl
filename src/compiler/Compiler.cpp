#include "Compiler.h"

namespace compiler {
    Compiler::Compiler() :
            ScopeGen(semantics::ScopeGenerator(&Global)) {

    }

    void Compiler::setBackend(Backend B)
    {
        switch (B)
        {
#ifdef CCPP
            case Backend::CPP:
                CodeGen = make_unique<codegen::CCodeGenerator>(*this);
                break;
#endif
#ifdef CGNUASM
            case Backend::GNUASM:
                CodeGen = make_unique<codegen::GasCodeGenerator>(*this);
                break;
#endif
#ifdef CHASKELL
            case Backend::HASKELL:
                CodeGen = make_unique<codegen::HCodeGenerator>(*this);
                break;
#endif
#ifdef CLLVM
            case Backend::LLVM:
                CodeGen = make_unique<codegen::LLVMCodeGenerator>(*this);
                break;
#endif
            case Backend::PPRINTER:
                CodeGen = make_unique<codegen::Printer>(*this);
                break;
            default:
                throw runtime_error("Not a valid backend!");
        }
    }

    int Compiler::compile() {

        if (!parseFiles())
            return 1;

        /*
        if (!accept(ScopeGen))
            return 2;

        if (!accept(TypeChecker))
            return 3;

        if (!accept(Optimizer))
            return 4;

        if (!accept(*CodeGen))
            return 5;
         */

        return 0;
    }
}

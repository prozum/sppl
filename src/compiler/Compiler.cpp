#include "Compiler.h"

namespace compiler {
    Compiler::Compiler() :
            scope_generator(semantics::ScopeGenerator(&Global))
    {

    }

    void Compiler::set_backend(Backend B)
    {
        switch (B)
        {
#ifdef CCPP
            case Backend::CPP:
                generator = make_unique<codegen::CCodeGenerator>(*this);
                break;
#endif
#ifdef CGNUASM
            case Backend::GNUASM:
                generator = make_unique<codegen::GasCodeGenerator>(*this);
                break;
#endif
#ifdef CHASKELL
            case Backend::HASKELL:
                generator = make_unique<codegen::HCodeGenerator>(*this);
                break;
#endif
#ifdef CLLVM
            case Backend::LLVM:
                generator = make_unique<codegen::LLVMCodeGenerator>(*this);
                break;
#endif
            case Backend::PPRINTER:
                generator = make_unique<codegen::Printer>(*this);
                break;
            default:
                throw runtime_error("Not a valid backend!");
        }
    }

    int Compiler::compile() {

        if (!parseFiles())
            return 1;

        cout << Prog->str() << endl;

        /*
        if (!accept(scope_generator))
            return 2;

        if (!accept(type_checker))
            return 3;

        if (!accept(optimizer))
            return 4;

        if (!accept(*generator))
            return 5;
        */

        return 0;
    }


}

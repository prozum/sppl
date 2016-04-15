#include "Compiler.h"

namespace compiler {
    Compiler::Compiler() :
            scope_generator(semantics::ScopeGenerator(&global)) {

    }

    void Compiler::set_backend(Backend backend)
    {
        switch (backend)
        {
#ifdef CCPP
            case Backend::CPP:
                generator = make_unique<codegen::CCodeGenerator>(*this);
                break;
#endif
#ifdef CGNUASM
            case Backend::GNUASM:
                generator = make_unique<codegen::GasCodeGenerator>(driver);
                break;
#endif
#ifdef CHASKELL
            case Backend::HASKELL:
                generator = make_unique<codegen::HCodeGenerator>(*output);
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
                throw runtime_error("Not a valid backend");
        }
    }

    int Compiler::compile() {

        if (!parse_files())
            return 1;

        if (!accept(scope_generator))
            return 2;

        if (!accept(type_checker))
            return 3;

        if (!accept(optimizer))
            return 4;

        if (!accept(*generator))
            return 5;

        return 0;
    }


}

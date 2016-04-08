#include "Compiler.h"

namespace compiler {
    Compiler::Compiler(shared_ptr<istream> in,
                       shared_ptr<ostream> out,
                       shared_ptr<ostream> hout)
            : input(in),
              output(out),
              header_output(hout),
              scope_generator(semantics::ScopeGenerator(driver.global.get())) {

    }

    void Compiler::set_backend(Backend backend)
    {
        switch (backend)
        {
#ifdef CCPP
            case Backend::CPP:
                generator = make_unique<codegen::CCodeGenerator>(output, header_output);
                break;
#endif
#ifdef CGNUASM
            case Backend::GNUASM:
                generator = make_unique<codegen::GasCodeGenerator>(*output);
                break;
#endif
#ifdef CHASKELL
            case Backend::HASKELL:
                generator = make_unique<codegen::HCodeGenerator>(*output);
                break;
#endif
#ifdef CLLVM
            case Backend::LLVM:
                generator = make_unique<codegen::LLVMCodeGenerator>(output);
                break;
#endif
            case Backend::PPRINTER:
                generator = make_unique<codegen::Printer>(output);
            default:
                throw "Not a valid backend";
        }
    }

    int Compiler::compile() {

        driver.parse_stream(*input);
        if (driver.program == nullptr)
            return 1;

        //case_checker.visit(*driver.program);
        scope_generator.visit(*driver.program);
        if (scope_generator.HasError())
            return 2;

        type_checker.visit(*driver.program);
        if (type_checker.HasError())
            return 3;

        generator->visit(*driver.program);
        return 0;
    }
}

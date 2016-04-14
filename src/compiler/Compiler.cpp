#include "Compiler.h"

namespace compiler {
    Compiler::Compiler() :
            scope_generator(semantics::ScopeGenerator(driver.global.get())) {

    }

    void Compiler::set_backend(Backend backend)
    {
        switch (backend)
        {
#ifdef CCPP
            case Backend::CPP:
                generator = make_unique<codegen::CCodeGenerator>(driver);
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
                generator = make_unique<codegen::LLVMCodeGenerator>(driver);
                break;
#endif
            case Backend::PPRINTER:
                generator = make_unique<codegen::Printer>(driver);
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
        if (scope_generator.hasError())
            return 2;

        type_checker.visit(*driver.program);
        if (type_checker.hasError())
            return 3;

        optimizer.visit(*driver.program);

        generator->visit(*driver.program);
        return 0;
    }

    void Compiler::set_output(string out) {
        //output =  ofstream(out);
        driver.codeout = output;
        driver.codeout = output;
    }

    void Compiler::set_header_output(string hout) {
        header_output =  ofstream(hout);
    }
}

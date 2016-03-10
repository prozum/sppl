#include "Compiler.h"

#include "CppCodeGenerator.h"
#include "GasCodeGenerator.h"
#include "HCodeGenerator.h"
#include "Printer.h"

namespace compiler {

    Compiler::Compiler(istream* in, ostream* out) : input(in), output(out)
    {
    }

    Compiler::Compiler(istream *in, ostream *out, ostream *hout) : input(in), output(out), header_output(hout) {

    }

    void Compiler::set_backend(Backend backend)
    {

        switch (backend)
        {
            case Backend::CPP:
                generator = make_unique<codegen::CppCodeGenerator>(*output);
                break;
            case Backend::GNUASM:
                generator = make_unique<codegen::GasCodeGenerator>(*output);
                break;
            case Backend::HASKELL:
                generator = make_unique<codegen::HCodeGenerator>(*output);
                break;
            case Backend::PPRINTER:
                generator = make_unique<codegen::Printer>(*output);
        }
    }

    void Compiler::compile() {

        driver.parse_stream(*input);

        generator->visit(driver.main);
    }

}

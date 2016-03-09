#include "Compiler.h"


namespace compiler {

    Compiler::Compiler(std::istream *in, std::ostream *out)
    {
        input = in;
        output = out;
    }

    void Compiler::set_codegener

    void Compiler::compile() {

        driver.parse_stream(input);

        generator->output = output;

        generator->visit(driver.main);
    }

    T Compiler::generator(ostream &out) {
        return nullptr;
    }
}

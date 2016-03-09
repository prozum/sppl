#include "Compiler.h"

namespace compiler {

    Compiler::Compiler(std::istream &in, std::ostream &out) : input(in), output(out)
    {
    }

    template <class T>
    void Compiler::set_codegenerator()
    {
        generator = make_unique<T>(input);
    }

    void Compiler::compile() {

        driver.parse_stream(input);

        generator->visit(*driver.main);
    }
}

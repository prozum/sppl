#include <gnuasm/GasCodeGenerator.h>
#include "Compiler.h"

#include "Driver.h"
#include "Printer.h"

namespace compiler {

    void Compiler::compile(std::istream &input,
                           std::ostream &output) {
        parser::Driver driver;

        driver.parse_stream(input);

        GasCodeGenerator gen(output);
        gen.visit(driver.main);

        // visitor::Printer v;

        // v.visit(driver.main);

        // std::cout << v.res << std::endl;
    }

    Compiler::Compiler() {

    }
}

#include "Compiler.h"

#include "Driver.h"
#include "Printer.h"

namespace compiler {

    void Compiler::compile(std::istream &input,
                           std::ostream &output) {
        parser::Driver driver;

        driver.parse_stream(input);

        visitor::Printer v;

        v.visit(driver.main);

        std::cout << v.res << std::endl;
    }

    Compiler::Compiler() {

    }
}
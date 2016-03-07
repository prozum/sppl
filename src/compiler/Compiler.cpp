#include "Compiler.h"

#include "Driver.h"
#include "HPInkJetVisitor.h"

namespace compiler {

    void Compiler::compile(std::istream &input,
                           std::ostream &output) {
        parser::Driver driver;

        driver.parse_stream(input);

        visitor::HPInkJetVisitor v;

        v.visit(driver.main);

        std::cout << v.res << std::endl;
    }

    Compiler::Compiler() {

    }
}
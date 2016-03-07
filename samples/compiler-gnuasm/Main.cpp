#include "Driver.h"
#include "GasCodeGenerator.h"
#include <iostream>

int main(int argc, char* argv) {
    parser::Driver driver;
    GasCodeGenerator gen(std::cout);
    driver.parse_string("def f : Int | x = 5");
    gen.visit(driver.main);
    return 0;
}
#include "Driver.h"
#include "GasCodeGenerator.h"
#include <iostream>

int main(int argc, char* argv) {
    parser::Driver driver;
    GasCodeGenerator gen(std::cout);
    driver.parse_string("def main : Int | = 2 + 2");
    gen.visit(driver.main);
    return 0;
}
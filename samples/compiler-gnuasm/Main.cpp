#include "Driver.h"
#include "GasCodeGenerator.h"
#include <iostream>

int main(int argc, char* argv) {
    parser::Driver driver;
    GasCodeGenerator gen(std::cout);
    driver.parse_string("def main : Int | = 2 + 2"
                                "def func : Int->Int->Int | 1 2 = 3 | 1 n = 2 * n | n m = n * m");
    gen.visit(driver.main);

    return 0;
}
#include "Driver.h"
#include "HCodeGenerator.h"
#include <iostream>

int main()
{
    parser::Driver driver;

    HCodeGenerator gen(std::cout);

    driver.parse_string("def f : Int -> Int | x = 5 * x");

    gen.visit(driver.main);

    return 0;
}

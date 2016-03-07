#include "Driver.h"
#include "HCodeGenerator.h"
#include <iostream>

int main()
{
    parser::Driver driver;

    HCodeGenerator gen(std::cout);

    driver.parse_string("def f : Int | x = 5");

    gen.visit(driver.main);

    return 0;
}

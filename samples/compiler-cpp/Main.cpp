#include "Driver.h"
#include "CppCodeGenerator.h"
#include <iostream>

int main()
{
    parser::Driver driver;

    CCodeGenerator gen(std::cout);

    driver.parse_string("def f : Int | x = 5");

    gen.visit(driver.main);

    return 0;
}

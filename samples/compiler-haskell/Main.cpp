#include "Driver.h"
#include "HCodeGenerator.h"
#include <iostream>

int main()
{
    parser::Driver driver;
    HCodeGenerator gen(std::cout);

    driver.parse_stream(cin);
    gen.visit(driver.main);

    return 0;
}

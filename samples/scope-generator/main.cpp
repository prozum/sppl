//
// Created by hejsil on 3/4/16.
//

#include <typeinfo>
#include <iostream>
#include <string>
#include <Scope.h>
#include "Driver.h"
#include "Parser.h"
#include "ScopeGenerator.h"

void print_scopes(Scope *scope);

int main(int argc, char** argv) {
    parser::Driver driver;
    ScopeGenerator generator;

    driver.parse_file(argv[1]);
    generator.visit(driver.main);
    print_scopes(generator.res);

    return 0;
}

int tap = 0;
std::string get_taps() {
    std::string res = "";

    for (int i = 0; i < tap; ++i) {
        res.append("\t");
    }

    return res;
}

void print_scopes(Scope *scope) {
    std::cout << get_taps() << "-";

    for (auto var : scope->decls) {
        std::cout << "(Id: " << var.first << " Type: " << ") ";
    }
    std::cout << std::endl;

    tap++;

    for (auto sc : scope->children) {
        print_scopes(sc);
    }

    tap--;
}

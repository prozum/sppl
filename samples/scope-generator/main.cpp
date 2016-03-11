//
// Created by hejsil on 3/4/16.
//
/*
#include <typeinfo>
#include <iostream>
#include <string>
#include <Scope.h>
#include "Driver.h"
#include "Parser.h"
#include "ScopeGenerator.h"
#include "TypeChecker.h"

std::string get_type(Type *type);
void print_scopes(Scope *scope);

int main(int argc, char** argv) {
    parser::Driver driver;
    ScopeGenerator generator;
    TypeChecker typeChecker;

    driver.parse_file(argv[1]);
    generator.visit(driver.main);
    typeChecker.visit(driver.main);

    //print_scopes(generator.res);

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

std::string print_collection(Type *node, const std::string split) {
    std::string res = "";

    if (node->types.size() != 0) {
        for (int i = 0; i < node->types.size() - 1; ++i) {
            res.append(get_type(node->types[i]));
            res.append(split);
        }

        res.append(get_type(node->types.back()));
    }
}

std::string get_type(Type *type) {
    std::string res = "";

    switch (type->type) {
        case INT:
            res.append("Int");
            break;
        case FLOAT:
            res.append("Float");
            break;
        case BOOL:
            res.append("Bool");
            break;
        case CHAR:
            res.append("Char");
            break;
        case STRING:
            res.append("String");
            break;
        case LIST:
            res.append("[");
            res.append(get_type(type->types[0]));
            res.append("]");
            break;
        case TUPLE:
            res.append("(");
            print_collection(type, ", ");
            res.append(")");
            break;
        case SIGNATURE:
            res.append("(");
            print_collection(type, " -> ");
            res.append(")");
            break;
        default:
            break;
    }

    return res;
}

void print_scopes(Scope *scope) {
    std::cout << get_taps() << "-";

    for (auto var : scope->decls) {
        std::cout << "(Id: " << var.first << " Type: " << get_type(var.second) << ") ";
    }
    std::cout << std::endl;

    tap++;

    for (auto sc : scope->children) {
        print_scopes(sc);
    }

    tap--;
}
 */

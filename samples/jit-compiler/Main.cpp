#include "Compiler.h"
#include "SpplJit.h"

#include <iostream>
#include <memory>
#include <istream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {

    SpplJit::init_llvm();

    std::stringstream input;

    SpplJit jit(&input, &cout);

    string tmp("");
    while (tmp.compare("q") != 0)
    {
        cout << "Input code:";
        jit.eval("def f: Int -> Int\n| 1 = 10\n| 2 = 20" + tmp);
        cin >> tmp;
        jit.eval("def expr: Int -> Int\n| = " + tmp);
        cout << endl << endl;
    }
}

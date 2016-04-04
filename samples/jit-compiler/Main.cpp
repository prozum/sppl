#include "Compiler.h"
#include "SpplJit.h"

#include <iostream>
#include <memory>
#include <istream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {

    SpplJit::Init_llvm();

    SpplJit jit(&cout);

    string input("");
    while (input.compare("q") != 0)
    {
        cout << "input: ";
        cin >> input;
        jit.Eval(input);
        cout << endl << endl;
    }
}

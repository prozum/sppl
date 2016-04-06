#include "Compiler.h"
#include "SpplJit.h"

#include <iostream>
#include <memory>
#include <istream>
#include <sstream>
#include <string>

using namespace jit;

int main(int argc, char *argv[]) {

    SpplJit::Init_llvm();

    shared_ptr<ostream> ptr(&cout);
    SpplJit jit(ptr);

    string input("");
    string input_case;
    jit.Eval("def test: Int\n| = 2*2");
    while (input.compare("q") != 0)
    {
        cout << ">>> ";
        cin >> input;

        if (!input.compare(0, 3, "def")) {
            do
            {
                cout << "... ";
                cin >> input_case;

            }
            while(!input_case.compare(0, 2, "\t|"));

        }

        //jit.Eval(input);
        cout << endl << endl;
    }
}

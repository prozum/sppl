#include "Compiler.h"
#include "SpplJit.h"

#include <regex>

using namespace jit;

int main(int argc, char *argv[]) {

    SpplJit::Init_llvm();

    shared_ptr<ostream> ptr(&cout);
    SpplJit jit(ptr);

    string input("");
    string input_case("");
    //jit.Eval("def test: Int\n| = 2*2");
    while (input.compare("q") != 0)
    {
        cout << ">>> ";
        getline(cin, input);

        if (!input.compare(0, 3, "def")) {
            do
            {
                input += input_case;
                cout << "... ";
                getline(cin, input_case);
            }
            while(!(input_case.compare(0, 1, "|") && input_case.compare(0, 2, "\t|")));
        }

        jit.Eval(input);
        cout << endl << endl;
    }
    //ptr.re;
}

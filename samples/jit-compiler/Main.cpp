#include "Compiler.h"
#include "SpplJit.h"

#include <regex>

using namespace jit;

int main(int argc, char *argv[]) {

    SpplJit::Init_llvm();
    SpplJit jit;

    if (argc > 1) {
        std::ifstream f(argv[1]);
        if (!f.good()) {
            std::cerr << "sppl: can't open file: '" << argv[1] << "'" << std::endl;
            return 1;
        }

        std::stringstream buffer;
        buffer << f.rdbuf();

        jit.Eval(buffer.str());
        return 0;
    }

    string input("");
    string input_case("");
    while (true)
    {
        cout << ">>> ";
        getline(cin, input);

        if (input.compare("quit") == 0 ||
            input.compare("exit") == 0)
            return 0;

        if (input.compare("") == 0) {
            cout << "Use 'quit' or 'exit' to leave" << endl;
        } else {
            if (!input.compare(0, 3, "def")) {
                do {
                    input += input_case;
                    cout << "... ";
                    getline(cin, input_case);
                }
                while (!(input_case.compare(0, 1, "|") && input_case.compare(0, 2, "\t|")));
            }

            jit.Eval(input);
        }
    }
}

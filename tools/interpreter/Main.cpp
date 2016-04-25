#include "Compiler.h"
#include "SpplJit.h"

#include <regex>

using namespace std;
using namespace jit;

int main(int argc, char *argv[]) {

    SpplJit::initLLVM();
    SpplJit JIT;

    if (argc > 1) {
        std::ifstream f(argv[1]);
        if (!f.good()) {
            std::cerr << "sppl: can't open file: '" << argv[1] << "'" << std::endl;
            return 1;
        }

        std::stringstream Buffer;
        Buffer << f.rdbuf();

        return JIT.eval(Buffer.str());
    }

    string Input("");
    string InputCase("");
    while (true)
    {
        cout << ">>> ";
        getline(cin, Input);

        if (Input.compare("quit") == 0 ||
            Input.compare("exit") == 0)
            return 0;

        if (Input.compare("") == 0) {
            cout << "Use 'quit' or 'exit' to leave" << endl;
        } else {
            if (!Input.compare(0, 3, "def")) {
                do {
                    Input += InputCase + "\n";
                    cout << "... ";
                    getline(cin, InputCase);
                }
                while (!(InputCase.compare(0, 1, "|") && InputCase.compare(0, 2, "\t|")));
            }

            JIT.eval(Input);
        }
    }
}

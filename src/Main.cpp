#include "Compiler.h"
#include <fstream>
#include <cstring>

using namespace std;
using namespace compiler;

int main(int argc, char *argv[])
{
    Backend backend = Backend::PPRINTER;
    vector<string> input;
    string output("a.out");
    string header_output("a.h");

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"--help") == 0)
        {
            cout << "Usage: " << argv[0] << "[options] <inputs>" << endl;
            cout << "Options:" << endl;
            cout << "  --output         -o     Output binary" << endl;
            cout << "  --header-output  -ho    Output header" << endl;
#ifdef CCPP
            cout << "  --cpp                   C++ backend" << endl;
#endif
#ifdef CGNUASM
            cout << "  --gnuasm                GNU Assembly backend" << endl;
#endif
#ifdef CHASKELL
            cout << "  --haskell               Haskell backend" << endl;
#endif
#ifdef CLLVM
            cout << "  --llvm                  LLVM backend" << endl;
#endif
            cout << "  --pprinter              Pretty printer backend" << endl;
            return 0;
        }
#ifdef CCPP
        else if (strcmp(argv[i],"--cpp") == 0)
            backend = Backend::CPP;
#endif
#ifdef CGNUASM
        else if (strcmp(argv[i],"--gnuasm") == 0)
            backend = Backend::GNUASM;
#endif
#ifdef CHASKELL
        else if (strcmp(argv[i],"--haskell") == 0)
            backend = Backend::GNUASM;
#endif
#ifdef CLLVM
        else if (strcmp(argv[i],"--llvm") == 0)
            backend = Backend::LLVM;
#endif
        else if (strcmp(argv[i],"--pprinter") == 0)
            backend = Backend::PPRINTER;
        else if (strcmp(argv[i],"--output") == 0 || strcmp(argv[i],"-o") == 0)
            output = argv[++i];
        else if (strcmp(argv[i],"--header-output") == 0 || strcmp(argv[i],"-ho") == 0)
            header_output = argv[++i];
        else
            input.push_back(argv[i]);
    }

    if (input.size() == 0)
    {
        cerr << "No input files" << endl;
        return 1;
    }

    ifstream in(input[0]);
    ofstream out(output);
    ofstream hout(header_output);

    compiler::Compiler compiler(&in, &out, &hout);
    compiler.set_backend(backend);

    compiler.compile();

    return 0;
}

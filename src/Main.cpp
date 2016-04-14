#include "Compiler.h"
#include <fstream>
#include <cstring>

using namespace std;
using namespace compiler;

void print_help(char *program_name)
{
    cout << "Usage: " << program_name << " [options] <inputs>" << endl;
    cout << "Options:" << endl;
    cout << "  --help           -h          Show available options" << endl << endl;
    cout << "  --output         -o  <file>  Set binary output file" << endl;
    cout << "  --header-output  -ho <file>  Set header output file" << endl << endl;
#ifdef CCPP
    cout << "  --cpp                        C++ backend" << endl;
#endif
#ifdef CGNUASM
    cout << "  --gnuasm                     GNU Assembly backend" << endl;
#endif
#ifdef CHASKELL
    cout << "  --haskell                    Haskell backend" << endl;
#endif
#ifdef CLLVM
    cout << "  --llvm                       LLVM backend" << endl;
#endif
    cout << "  --pprinter                   Pretty printer backend" << endl;
}

int main(int argc, char *argv[])
{
    Backend backend = Backend::PPRINTER;
    vector<string> input;
    string output("a.out");
    string header_output("a.h");

    if (argc == 1) {
        print_help(argv[0]);
        return 0;
    }

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
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
            if (i < argc - 1)
                output = argv[++i];
            else {
                cerr << "No output file specified" << endl;
                return 2;
            }
        else if (strcmp(argv[i],"--header-output") == 0 || strcmp(argv[i],"-ho") == 0)
            if (i < argc - 1)
                header_output = argv[++i];
            else {
                cerr << "No output header specified" << endl;
                return 3;
            }
         else
            input.push_back(argv[i]);
    }

    if (input.size() == 0)
    {
        cerr << "No input files" << endl;
        return 1;
    }

    auto in = make_shared<ifstream>(input[0]);
    auto out = make_shared<ofstream>(output);
    auto hout = make_shared<ofstream>(header_output);

    compiler::Compiler compiler(in, out, hout);
    compiler.set_backend(backend);

    int res = compiler.compile();

    switch (res) {
        case 2:
            compiler.scope_generator.outError(cerr);
            break;
        case 3:
            compiler.type_checker.outError(cerr);
            break;
    }

    return res;
}

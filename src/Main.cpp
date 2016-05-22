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
    cout << "  --c                          C backend" << endl;
    cout << "  --cpar                       Parallel C backend" << endl;
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
    Backend BE = Backend::PPRINTER;
    vector<string> Inputs;
    string Output("a.out");
    string HeaderOutput("a.h");
    int OptimizationLevel = 0;

    if (argc == 1) {
        print_help(argv[0]);
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        string Arg = string(argv[i]);
        if (Arg.compare("--help") == 0 || Arg.compare("-h") == 0) {
            print_help(argv[0]);
            return 0;
        }
#ifdef CCPP
        else if (Arg.compare("--cpar") == 0)
            BE = Backend::CPAR;
        else if (Arg.compare("--c") == 0)
            BE = Backend::C;
#endif
#ifdef CGNUASM
        else if (Arg.compare("--gnuasm") == 0)
            backend = Backend::GNUASM;
#endif
#ifdef CHASKELL
        else if (Arg.compare("--haskell") == 0)
            backend = Backend::GNUASM;
#endif
#ifdef CLLVM
        else if (Arg.compare("--llvm") == 0)
            BE = Backend::LLVM;
#endif
        else if (Arg.compare("--pprinter") == 0)
            BE = Backend::PPRINTER;
        else if (Arg.compare("--output") == 0 || Arg.compare("-o") == 0)
            if (i < argc - 1)
                Output = argv[++i];
            else {
                cerr << "No output file specified" << endl;
                return 2;
            }
        else if (Arg.compare("--header-output") == 0 || Arg.compare("-ho") == 0)
            if (i < argc - 1)
                HeaderOutput = argv[++i];
            else {
                cerr << "No output header specified" << endl;
                return 3;
            }
        else if (Arg.compare("--optimizaton-level") == 0 || Arg.compare(0, 3,"-ol") == 0)
            if (i < argc - 1 && strlen(argv[i+1]) == 1 && isdigit(argv[i+1][0])) {
                OptimizationLevel = stoi(argv[++i]);
            } else {
                cerr << "No valid optimization level specified" << endl;
                return 4;
            }
        else if (Arg.compare(0, 2, "--") == 0) {
            cerr << "No such option: " << Arg << endl;
            cerr << "For help use --help" << endl;
            return 5;
        }
        else {
            Inputs.push_back(Arg);
        }
    }

    if (Inputs.size() == 0)
    {
        cerr << "No input files" << endl;
        return 1;
    }

    Compiler compiler;
    compiler.setOutput(Output);
    compiler.setHeaderOutput(HeaderOutput);
    compiler.setBackend(BE);
    compiler.setOptimizationLevel(OptimizationLevel);

    return compiler.compile(Inputs);
}

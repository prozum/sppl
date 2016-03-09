#include "compiler/Compiler.h"

#include <fstream>

using namespace std;
using namespace compiler;

int main(int argc, char *argv[])
{
    if (argc != 3)
        return  1;

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    Compiler::compile(in, out);

    return 0;
}

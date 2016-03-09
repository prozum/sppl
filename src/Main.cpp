#include "Compiler.h"

#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
        return  1;

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    Compiler compiler(in, out);

    return 0;
}

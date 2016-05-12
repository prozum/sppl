#include "LLVMCodeGen.h"

using namespace codegen;
using namespace std;

void LLVMCodeGen::addPrefix(string Prefix) {
    Prefixes.push_back(make_pair(Prefix, 0));
}

void LLVMCodeGen::delPrefix() {
    Prefixes.pop_back();
}

string LLVMCodeGen::getPrefix() {
    string Res;
    for (auto Prefix = Prefixes.cbegin(); Prefix < Prefixes.cend(); ++Prefix) {
        Res += Prefix->first;

        Res += to_string(Prefix->second);

        if (next(Prefix) != Prefixes.cend()) {
            Res += "_";
        }
    }
    return Res;
}

void LLVMCodeGen::stepPrefix() {
    Prefixes.back().second++;
}

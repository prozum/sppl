#include "LLVMCodeGen.h"

using namespace codegen;
using namespace std;

void LLVMCodeGen::addPrefix(string Prefix, bool Numbered) {
    if (Numbered)
        Prefixes.push_back(make_pair(Prefix, 0));
    else
        Prefixes.push_back(make_pair(Prefix, -1));
}

void LLVMCodeGen::delPrefix() {
    Prefixes.pop_back();
}

string LLVMCodeGen::getPrefix() {
    return getPrefix("");
}

string LLVMCodeGen::getPrefix(string Str) {
    string Res;
    for (auto Prefix = Prefixes.cbegin(); Prefix < Prefixes.cend(); ++Prefix) {
        Res += Prefix->first;

        if (Prefix->second != -1)
            Res += to_string(Prefix->second);

        if (next(Prefix) != Prefixes.cend()) {
            Res += "_";
        }
    }

    return Str.empty() ? Res : Res + "_" + Str;
}

void LLVMCodeGen::stepPrefix() {
    Prefixes.back().second++;
}

#include "Scope.h"

using namespace std;
using namespace common;

Scope::Scope(common::Scope *Scp) : Parent(Scp) {}

bool Scope::tryGetDecl(std::string Id, Type &OutType) {
    auto Got = Decls.find(Id);

    if (Got != Decls.end()) {
        OutType = Got->second;
        return true;
    }

    if (Parent)
        return Parent->tryGetDecl(Id, OutType);

    return false;
}

bool Scope::tryGetADT(std::string Id, AlgebraicDT *&OutADT) {
    auto Got = Types.find(Id);

    if (Got != Types.end()) {
        OutADT = &Got->second;
        return true;
    }

    if (Parent)
        return Parent->tryGetADT(Id, OutADT);

    OutADT = nullptr;
    return false;
}

bool Scope::tryGetCon(std::string Id, Product *&OutProduct) {
    auto Got = Constructors.find(Id);

    if (Got != Constructors.end()) {
        OutProduct = &Got->second;
        return true;
    }

    if (Parent)
        return Parent->tryGetCon(Id, OutProduct);

    OutProduct = nullptr;
    return false;
}

bool Scope::tryGetGenerated(std::string Id, Type Ty, std::string *OutName) {
    auto Got = GeneratedGenerics.find(Id);

    if (Got != GeneratedGenerics.end()) {
        auto Got2 = Got->second.find(Ty);

        if (Got2 != Got->second.end()) {
            OutName = &Got2->second;
            return true;
        }

        return false;
    }

    if (Parent)
        return Parent->tryGetGenerated(Id, Ty, OutName);

    return false;
}

bool Scope::tryGetGenFunc(string Id, Function *&OutFunc) {
    auto Got = GenericFuncs.find(Id);

    if (Got != GenericFuncs.end()) {
        OutFunc = Got->second.get();
        return true;
    }

    if (Parent)
        return Parent->tryGetGenFunc(Id, OutFunc);

    OutFunc = nullptr;
    return false;
}

bool Scope::tryGetGenADT(string Id, AlgebraicDT *&OutADT) {
    auto Got = GenericADT.find(Id);

    if (Got != GenericADT.end()) {
        OutADT = Got->second.get();
        return true;
    }

    if (Parent)
        return Parent->tryGetGenADT(Id, OutADT);

    OutADT = nullptr;
    return false;
}
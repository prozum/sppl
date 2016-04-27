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
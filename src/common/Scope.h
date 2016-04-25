#pragma once

#include "Node.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace common {
class Scope {
  public:
    unordered_map<string, Type> Decls;
    unordered_map<string, AlgebraicDT &> Types;
    unordered_map<string, Product &> Constructors;
    unordered_map<string, shared_ptr<AlgebraicDT>> GenericADT;
    unordered_map<string, shared_ptr<Function>> GenericFuncs;
    unordered_map<string, unordered_map<Type, string>> GeneratedGenerics;

    Scope *Parent;
    std::vector<unique_ptr<Scope>> Children;

    Scope() : Scope(nullptr) {}
    Scope(Scope *Scp);

    bool tryGetDecl(string Id, Type &OutType);
    bool tryGetADT(string Id, AlgebraicDT *&OutADT);
    bool tryGetCon(string Id, Product *&OutProduct);
    bool tryGetGenFunc(string Id, Function *&OutFunc);
    bool tryGetGenADT(string Id, AlgebraicDT *&OutADT);
    bool tryGetGenerated(std::string Id, Type Ty, std::string *OutName);
};
}

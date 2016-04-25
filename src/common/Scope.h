#pragma once

#include "Node.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace common {
class Scope {
  public:
    std::unordered_map<std::string, Type> Decls;
    std::unordered_map<std::string, AlgebraicDT &> Types;
    std::unordered_map<std::string, Product &> Constructors;
    std::unordered_map<std::string, std::shared_ptr<AlgebraicDT>> GenericADT;
    std::unordered_map<std::string, std::shared_ptr<Function>> GenericFuncs;
    std::unordered_map<std::string, std::unordered_map<Type, std::string>> GeneratedGenerics;

    Scope *Parent;
    std::vector<std::unique_ptr<Scope>> Children;

    Scope() : Scope(nullptr) {}
    Scope(Scope *Scp);

    bool tryGetDecl(std::string Id, Type &OutType);
    bool tryGetADT(std::string Id, AlgebraicDT *&OutADT);
    bool tryGetCon(std::string Id, Product *&OutProduct);
    bool tryGetGenFunc(std::string Id, Function *&OutFunc);
    bool tryGetGenADT(std::string Id, AlgebraicDT *&OutADT);
    bool tryGetGenerated(std::string Id, Type Ty, std::string *OutName);
};
}

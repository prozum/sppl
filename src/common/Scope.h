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

    Scope *Parent;
    std::vector<std::unique_ptr<Scope>> Children;

    Scope() : Scope(nullptr) {}
    Scope(Scope *Scp);

    bool tryGetDecl(std::string Id, Type &OutType);
    bool tryGetADT(std::string Id, AlgebraicDT *&OutADT);
    bool tryGetCon(std::string Id, Product *&OutProduct);
};
}

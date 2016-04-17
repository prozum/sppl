#pragma once

#include "Node.h"

#include <vector>
#include <unordered_map>

namespace common {
    class Scope {
    public:
        std::unordered_map<std::string, Type> Decls;

        Scope* Parent;
        std::vector<unique_ptr<Scope>> Children;

        Scope() : Scope(nullptr) { }
        Scope(Scope* Scp);

        bool exists(std::string Id);
        Type getType(std::string Id);
    };
}

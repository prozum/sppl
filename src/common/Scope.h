#pragma once

#include <vector>
#include <unordered_map>
#include "Node.h"

namespace common {
    class Scope {
    public:
        std::unordered_map<std::string, Type *> decls;

        Scope *parent;
        std::vector<Scope *> children;

        Scope() : Scope(nullptr) { }
        Scope(Scope *p);

        bool exists(std::string id);
        Type *get_type(std::string id);
    };
}

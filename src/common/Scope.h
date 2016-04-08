#pragma once

#include <vector>
#include <unordered_map>
#include "Node.h"

namespace common {
    class Scope {
    public:
        std::unordered_map<std::string, shared_ptr<Type>> decls;

        Scope* parent;
        std::vector<shared_ptr<Scope>> children;

        Scope() : Scope(nullptr) { }
        Scope(Scope* p);

        bool exists(std::string id);
        shared_ptr<Type> get_type(std::string id);
    };
}

//
// Created by hejsil on 3/4/16.
//

#ifndef SPPL_SCOPE_H
#define SPPL_SCOPE_H

#include <vector>
#include <unordered_map>
#include "Node.h"

namespace common {
    class Scope {
    public:
        std::unordered_map<std::string, Type *> decls;
        std::unordered_map<std::string, std::string> real_ids;

        Scope *parent;
        std::vector<Scope *> children;

        Scope() : Scope(nullptr) { }
        Scope(Scope *p);

        bool exists(std::string id);
        string get_real_id(std::string id);
        Type *get_type(std::string id);
    };
}


#endif //SPPL_SCOPE_H

//
// Created by hejsil on 3/4/16.
//

#include "Scope.h"

common::Scope::Scope(common::Scope *p) {
    parent = p;
}

bool common::Scope::exists(std::string id) {
    auto got = decls.find(id);

    if (got != decls.end())
        return true;

    if (parent)
        return parent->exists(id);
    else
        return false;
}

common::Type *common::Scope::get_type(std::string id) {
    auto got = this->decls.find(id);

    if(got == this->decls.end()){
        return this->parent->get_type(id);
    } else {
        return got->second;
    }
}

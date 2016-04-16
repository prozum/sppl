#include "Scope.h"

namespace common {
    Scope::Scope(common::Scope *p)
            : Parent(p) { }

    bool common::Scope::exists(std::string id) {
        auto got = Decls.find(id);

        if (got != Decls.end())
            return true;

        if (Parent)
            return Parent->exists(id);
        else
            return false;
    }

    Type Scope::getType(std::string id) {
        auto got = this->Decls.find(id);

        if (got == this->Decls.end()) {
            return this->Parent->getType(id);
        } else {
            return got->second;
        }
    }
}
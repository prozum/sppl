#include "Scope.h"

namespace common {
    Scope::Scope(common::Scope *Scp)
            : Parent(Scp) { }

    bool common::Scope::exists(std::string Id) {
        auto got = Decls.find(Id);

        if (got != Decls.end())
            return true;

        if (Parent)
            return Parent->exists(Id);
        else
            return false;
    }

    Type Scope::getType(std::string Id) {
        auto got = this->Decls.find(Id);

        if (got == this->Decls.end()) {
            return this->Parent->getType(Id);
        } else {
            return got->second;
        }
    }
}
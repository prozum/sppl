#include "Scope.h"

namespace common {
    Scope::Scope(common::Scope *Scp)
            : Parent(Scp) { }

    bool common::Scope::exists(std::string Id) {
        auto Got = Decls.find(Id);

        if (Got != Decls.end())
            return true;

        if (Parent)
            return Parent->exists(Id);
        else
            return false;
    }

    Type Scope::getType(std::string Id) {
        auto Got = this->Decls.find(Id);

        if (Got == this->Decls.end()) {
            return this->Parent->getType(Id);
        } else {
            return Got->second;
        }
    }
}
#include "Scope.h"

namespace common {
    Scope::Scope(common::Scope *Scp)
            : Parent(Scp) { }

    bool common::Scope::declExists(std::string Id) {
        return exists(Id, Decls);
    }

    bool Scope::typeExists(std::string Id) {
        return exists(Id, Types);
    }

    bool Scope::conExists(std::string Id) {
        return exists(Id, Constructors);
    }

    template<class T>
    bool Scope::exists(std::string Id, unordered_map<string, T> Map) {
        auto Got = Map.find(Id);

        if (Got != Map.end())
            return true;

        if (Parent)
            return Parent->exists(Id, Map);
        else
            return false;
    }

    Type Scope::getDeclType(std::string Id) {
        return get(Id, Decls);
    }

    AlgebraicDT& Scope::getADT(std::string Id) {
        return get(Id, Types);
    }

    Product& Scope::getCon(std::string Id) {
        return get(Id, Constructors);
    }

    template<class T>
    T Scope::get(std::string Id, unordered_map<string, T> List) {
        auto Got = List.find(Id);

        if (Got == List.end()) {
            return this->Parent->get(Id, List);
        } else {
            return Got->second;
        }
    }
}
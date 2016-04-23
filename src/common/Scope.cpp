#include "Scope.h"

namespace common {
    Scope::Scope(common::Scope *Scp)
            : Parent(Scp) { }

    bool Scope::declExists(std::string Id) {
        if (Decls.find(Id) != Decls.end())
            return true;

        if (Parent)
            return Parent->declExists(Id);
        else
            return false;
    }

    bool Scope::typeExists(std::string Id) {
        if (Types.find(Id) != Types.end())
            return true;

        if (Parent)
            return Parent->typeExists(Id);
        else
            return false;
    }


    bool Scope::conExists(std::string Id) {
        if (Constructors.find(Id) != Constructors.end())
            return true;

        if (Parent)
            return Parent->conExists(Id);
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
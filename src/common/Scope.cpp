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
        auto Got = Decls.find(Id);

        if (Got == Decls.end()) {
            return Parent->getDeclType(Id);
        }

        return Got->second;
    }

    AlgebraicDT& Scope::getADT(std::string Id) {
        auto Got = Types.find(Id);

        if (Got == Types.end()) {
            return Parent->getADT(Id);
        }

        return Got->second;

    }

    Product& Scope::getCon(std::string Id) {
        auto Got = Constructors.find(Id);

        if (Got == Constructors.end()) {
            return Parent->getCon(Id);
        }

        return Got->second;
    }

}
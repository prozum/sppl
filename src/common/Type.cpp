#include "Type.h"

namespace common {

    Type::Type() { }

    Type::~Type() {
    }

    Type::Type(TypeId Id) :
            Id(Id) { }

    Type::Type(TypeId Id,
           Location Loc) :
            Id(Id),
            Loc(Loc) { }

    Type::Type(TypeId Id,
               vector<Type> Subtypes) :
                Id(Id),
                Subtypes(Subtypes) { }

    Type::Type(TypeId Id,
               vector<Type> Subtypes,
               Location Loc) :
                Id(Id),
                Subtypes(Subtypes),
                Loc(Loc) { }

    bool Type::operator==(const Type &Other) const {
        if (Id != Other.Id)
            return false;
        switch (Id) {
            case TypeId::LIST:
                return Subtypes[0] == Other.Subtypes[0];
            case TypeId::SIGNATURE:
            case TypeId::TUPLE:
                if (Subtypes.size() != Other.Subtypes.size())
                    return false;

                for (unsigned i = 0; i < Subtypes.size(); ++i) {
                    if (Subtypes[i] != Other.Subtypes[i]) {
                        return false;
                    }
                }
            default:
                return true;
        }
    }

    bool Type::operator!=(const Type &Other) const {
        return !(*this == Other);
    }

    Type Type::operator[](const size_t Index) {
        return  Subtypes[Index];
    }

    std::string Type::str() {
        switch (Id) {
            case TypeId::INT:
                return "Int";
            case TypeId::FLOAT:
                return "Float";
            case TypeId::BOOL:
                return "Bool";
            case TypeId::CHAR:
                return "Char";
            case TypeId::STRING:
                return "String";
            case TypeId::LIST:
                return "[" + Subtypes[0].str() + "]";
            case TypeId::TUPLE:
                return subtypeStr(", ");
            case TypeId::SIGNATURE:
                return subtypeStr(" -> ");
            case TypeId::EMPTYLIST:
                return "[]";
            default:
                throw runtime_error("This should not happen!");
        }
    }

    string Type::subtypeStr(const std::string Split) {
        string str("(");
        for (size_t i = 0; i < Subtypes.size(); i++) {
            str += Subtypes[i].str();
            if (i + 1 != Subtypes.size())
                str += Split;
        }

        return str + ")";
    }
}
#include "Type.h"

namespace common {

    Type::Type() { }

    Type::~Type() {
    }

    Type::Type(TypeId id) :
            Id(id) { }

    Type::Type(TypeId id,
           Location loc) :
            Id(id),
            Loc(loc) { }

    Type::Type(TypeId id,
               vector<Type> subtypes) :
                Id(id),
                Subtypes(subtypes) { }

    Type::Type(TypeId id,
               vector<Type> subtypes,
               Location loc) :
                Id(id),
                Subtypes(subtypes),
                Loc(loc) { }

    bool Type::operator==(const Type &other) const {
        if (Id != other.Id)
            return false;
        switch (Id) {
            case TypeId::LIST:
                return Subtypes[0] == other.Subtypes[0];
            case TypeId::SIGNATURE:
            case TypeId::TUPLE:
                if (Subtypes.size() != other.Subtypes.size())
                    return false;

                for (unsigned i = 0; i < Subtypes.size(); ++i) {
                    if (Subtypes[i] != other.Subtypes[i]) {
                        return false;
                    }
                }
            default:
                return true;
        }
    }

    bool Type::operator!=(const Type &other) const {
        return !(*this == other);
    }

    Type Type::operator[](const size_t i) {
        return  Subtypes[i];
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
                return "Unknown";
        }
    }

    string Type::subtypeStr(const std::string split) {
        string str("(");
        for (size_t i = 0; i < Subtypes.size(); i++) {
            str += Subtypes[i].str();
            if (i + 1 != Subtypes.size())
                str += split;
        }

        return str + ")";
    }


}
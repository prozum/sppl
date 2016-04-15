#include "Type.h"

namespace common {

    Type::Type() { }

    Type::~Type() {
    }

    Type::Type(TypeId id) :
            id(id) { }

    Type::Type(TypeId id,
           Location loc) :
            id(id),
            loc(loc) { }

    Type::Type(TypeId id,
               vector<Type> subtypes) :
                id(id),
                subtypes(subtypes) { }

    Type::Type(TypeId id,
               vector<Type> subtypes,
               Location loc) :
                id(id),
                subtypes(subtypes),
                loc(loc) { }

    bool Type::operator==(const Type &other) const {
        if (id != other.id)
            return false;
        switch (id) {
            case TypeId::LIST:
                return subtypes[0] == other.subtypes[0];
            case TypeId::SIGNATURE:
            case TypeId::TUPLE:
                if (subtypes.size() != other.subtypes.size())
                    return false;

                for (unsigned i = 0; i < subtypes.size(); ++i) {
                    if (subtypes[i] != other.subtypes[i]) {
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
        return  subtypes[i];
    }

    std::string Type::str() {
        switch (id) {
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
                return "[" + subtypes[0].str() + "]";
            case TypeId::TUPLE:
                return subtype_str(", ");
            case TypeId::SIGNATURE:
                return subtype_str(" -> ");
            case TypeId::EMPTYLIST:
                return "[]";
            default:
                return "Unknown";
        }
    }

    string Type::subtype_str(const std::string split) {
        string str("(");
        for (auto &type: this->subtypes) {
            str += type.str();
            if (type != this->subtypes.back())
                str += split;
        }

        return str + ")";
    }


}
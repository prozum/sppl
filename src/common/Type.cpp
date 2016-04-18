#include "Type.h"
#include "Node.h"

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
                return "FloatPattern";
            case TypeId::CHAR:
                return "CharPattern";
            case TypeId::LIST:
                return "[" + Subtypes[0].str() + "]";
            case TypeId::TUPLE:
                return strJoin(", ");
            case TypeId::SIGNATURE:
                return strJoin(" -> ");
            case TypeId::EMPTYLIST:
                return "[]";
            default:
                throw runtime_error("This should not happen!");
        }
    }

    string Type::strJoin(const std::string JoinStr) {
        string Str("(");
        for (size_t i = 0; i < Subtypes.size(); ++i) {
            Str += Subtypes[i].str();
            if (i + 1 != Subtypes.size())
                Str += JoinStr;
        }

        return "(" + Str + ")";
    }
}
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

    Type::Type(TypeId Id,
               vector<Type> Subtypes,
               string Name) :
            Id(Id),
            Subtypes(Subtypes),
            Name(Name) { }

    Type::Type(TypeId Id,
               vector<Type> Subtypes,
               string Name,
               Location Loc) :
            Id(Id),
            Subtypes(Subtypes),
            Name(Name),
            Loc(Loc) { }

    bool Type::operator==(const Type &Other) const {
        if (Id != Other.Id)
            return false;
        switch (Id) {
            case TypeId::GENERIC:
                return Name == Other.Name;
            case TypeId::CUSTOM:
                if (Name == Other.Name) {
                    if (Subtypes.size() != Other.Subtypes.size())
                        return false;

                    for (size_t i = 0; i < Subtypes.size(); ++i) {
                        if (Subtypes[i] != Other.Subtypes[i]) {
                            return false;
                        }
                    }
                } else {
                    return false;
                }
            case TypeId::LIST:
                return Subtypes[0] == Other.Subtypes[0];
            case TypeId::SIGNATURE:
            case TypeId::TUPLE:
                if (Subtypes.size() != Other.Subtypes.size())
                    return false;

                for (size_t i = 0; i < Subtypes.size(); ++i) {
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
            case TypeId::CHAR:
                return "Char";
            case TypeId::BOOL:
                return "Bool";
            case TypeId::STRING:
                return "String";
            case TypeId::LIST:
                return "[" + Subtypes[0].str() + "]";
            case TypeId::TUPLE:
                return strJoin(", ");
            case TypeId::SIGNATURE:
                return strJoin(" -> ");
            case TypeId::CUSTOM:
                return Name + " " + strJoin(" ");
            case TypeId::GENERIC:
                return Name;
            default:
                throw runtime_error("This should not happen!");
        }
    }

    string Type::strJoin(const std::string JoinStr) {
        string Str;
        for (size_t i = 0; i < Subtypes.size(); ++i) {
            if (Subtypes[i].Id == TypeId::SIGNATURE ||
                Subtypes[i].Id == TypeId::CUSTOM)
                Str += "(" + Subtypes[i].str() + ")";
            else
                Str += Subtypes[i].str();

            if (i + 1 != Subtypes.size())
                Str += JoinStr;
        }

        return Str;
    }
}
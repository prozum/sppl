#include "Type.h"
#include "Node.h"

using namespace std;
using namespace common;

Type Type::String = Type(TypeId::STRING, vector<Type> { Type(TypeId::CHAR) });

Type::Type() : Id(TypeId::UNKNOWN) {}

Type::~Type() {}

Type::Type(TypeId Id) : Id(Id) {}

Type::Type(TypeId Id, Location Loc) : Id(Id), Loc(Loc) {}

Type::Type(TypeId Id, vector<Type> Subtypes)
    : Id(Id), Subtypes(Subtypes) {}

Type::Type(TypeId Id, vector<Type> Subtypes, Location Loc)
    : Id(Id), Subtypes(Subtypes), Loc(Loc) {}

Type::Type(TypeId Id, vector<Type> Subtypes, string Name)
    : Id(Id), Subtypes(Subtypes), Name(Name) {}

Type::Type(TypeId Id, vector<Type> Subtypes, string Name, Location Loc)
    : Id(Id), Subtypes(Subtypes), Name(Name),
      Loc(Loc) {}

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

            return true;
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

bool Type::operator!=(const Type &Other) const { return !(*this == Other); }

Type Type::operator[](const size_t Index) { return Subtypes[Index]; }

std::string Type::str(bool isTop) {
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
        return "(" + strJoin(", ") + ")";
    case TypeId::SIGNATURE:
        if (isTop)
            return strJoin(" -> ");
        else
            return "(" + strJoin(" -> ") + ")";
    case TypeId::CUSTOM:
        return Name + " " + strJoin(" ");
    case TypeId::GENERIC:
        return Name;
    case TypeId::EMPTYLIST:
        return "[]";
    case TypeId::VOID:
        return "Void";
    case TypeId::UNKNOWN:
        return "_Unknown_";
    default:
        throw runtime_error("Something went wrong!");
    }
}

string Type::strJoin(const std::string JoinStr) {
    string Str;
    for (size_t i = 0; i < Subtypes.size(); ++i) {
        Str += Subtypes[i].str();

        if (i + 1 != Subtypes.size())
            Str += JoinStr;
    }

    return Str;
}

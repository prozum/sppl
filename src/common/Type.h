#pragma once

#include "Location.h"

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

namespace common {

	enum class TypeId {
		UNKNOWN,
        INT,
        FLOAT,
        CHAR,
		BOOL,
		STRING,
		LIST,
		TUPLE,
		SIGNATURE,
		GENERIC,
		CUSTOM
	};

	class Type {
	public:
		TypeId Id;
		vector<Type> Subtypes;
		string Name;
		Location Loc;

		Type();
		~Type();
		Type(TypeId Id);
		Type(TypeId Id, Location Loc);
		Type(TypeId Id, std::vector<Type> Subtypes);
		Type(TypeId Id, std::vector<Type> Subtypes, Location Loc);
		Type(TypeId Id, std::vector<Type> Subtypes, string Name);
		Type(TypeId Id, std::vector<Type> Subtypes, string Name, Location Loc);

		bool operator==(const Type &Other) const;
		bool operator!=(const Type &Other) const;
		Type operator[](const size_t Index);

		string str();
        string strJoin(string JoinStr);
	};
}

namespace std
{
	template <>
    struct hash<common::Type>
    {
        std::size_t operator()(const common::Type& Ty) const
        {
            size_t Res = hash<int>()(static_cast<int>(Ty.Id));

            for (auto &Subtype : Ty.Subtypes) {
                Res ^= (hash<common::Type>()(Subtype) << 1);
            }

            return Res;
        }
    };
}

#pragma once

#include "Location.h"

#include <vector>
#include <string>

using namespace std;

namespace common {

	enum class TypeId {
		UNKNOWN,
        INT,
        FLOAT,
        BOOL,
        CHAR,
		STRING,
		LIST,
		TUPLE,
		SIGNATURE,
		EMPTYLIST
	};

	class Type {
	public:
		TypeId id;
		vector<Type> subtypes;
		Location loc;

		Type(TypeId);
		Type(TypeId, Location);
		Type(TypeId, std::vector<Type>);
		Type(TypeId, std::vector<Type>, Location);

		bool operator==(const Type &other) const;
		bool operator!=(const Type &other) const;
		Type operator[](const size_t i);

		string str();
		string subtype_str(const std::string split);

	};

}

namespace std
{
	template <>
    struct hash<common::Type>
    {
        std::size_t operator()(const common::Type& type) const
        {
            size_t res = std::hash<int>()(static_cast<int>(type.id));

            for (auto &subtype: type.subtypes) {
                res ^= (hash<common::Type>()(subtype) << 1);
            }

            return res;
        }
    };
}

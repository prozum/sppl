#pragma once

#include <string>
#include <ostream>

using namespace std;

namespace common {

	class Location {
	public:
		Location();

		Location(size_t l, size_t c, std::string source);

		size_t l = 0;
		size_t c = 0;
		std::string source;

		Location *begin;
		Location *end;

	};

	std::ostream &operator<<(ostream &o, const Location &s);
}

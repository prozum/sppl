#pragma once

#include <string>
#include <ostream>

using namespace std;

namespace common {

	class Location {
	public:
		Location();

		Location(int l, int c, string src);

		int lines = 0;
		int columns = 0;
		string source;

		Location *begin;
		Location *end;

	};

	ostream &operator<<(ostream &o, const Location &s);
}

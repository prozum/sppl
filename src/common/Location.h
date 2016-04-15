#pragma once

#include <string>
#include <ostream>
#include <memory>

using namespace std;

namespace common {

	struct Position {

		Position() = default;
		Position(unsigned l, unsigned c, string src = "") :
			line(l), column(c), source(src) { }
		unsigned line = 1;
		unsigned column = 1;
		string source;
	};

	class Location {
	public:
		Location();
		Location(unsigned l, unsigned c, string src = "");

		void step();
		void columns (unsigned count = 1);
		void lines(unsigned count = 1);

		Position begin;
		Position end;
	};

	ostream &operator<<(ostream &o, const Location &s);
}

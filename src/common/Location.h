#pragma once

#include <string>
#include <ostream>
#include <memory>

using namespace std;

namespace common {

	struct Position {
		Position() = default;
		Position(unsigned l, unsigned c, string src = "") :
			Line(l), Column(c), Src(src) { }

		unsigned Line = 1;
		unsigned Column = 1;
		string Src;
	};

	class Location {
	public:
		Location();
		Location(unsigned l, unsigned c, string src = "");

		void step();
		void columns(unsigned count = 1);
		void lines(unsigned count = 1);
		void sources(string src = "");

		Position begin;
		Position end;
	};

	ostream &operator<<(ostream &o, const Location &s);
}

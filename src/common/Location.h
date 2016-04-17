#pragma once

#include <string>
#include <ostream>
#include <memory>

using namespace std;

namespace common {

	struct Position {
		Position() = default;
		Position(unsigned Line, unsigned Column, string Src) :
			Line(Line), Column(Column), Src(Src) { }

		unsigned Line = 1;
		unsigned Column = 1;
		string Src;
	};

	class Location {
	public:
		Location();
		Location(unsigned Line, unsigned Column, string Src);
		Location(const Location &Other);

		void step();
		void columns(unsigned Count = 1);
		void lines(unsigned Count = 1);
		void sources(string Src);

		Position begin;
		Position end;
	};

	ostream &operator<<(ostream &Out, const Location &Loc);
}

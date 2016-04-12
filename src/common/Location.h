#pragma once

#include <string>
#include <ostream>
#include <memory>

using namespace std;

namespace common {

	class Location {
	public:
		Location();
		//Location(const common::Location&);
		//Location(int l, int c, string src);
		Location(int l, int c);

		int lines = 0;
		int columns = 0;
		//string source;

		Location *begin;
		Location *end;
	};

	ostream &operator<<(ostream &o, const Location &s);
}

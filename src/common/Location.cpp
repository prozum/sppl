#include "Location.h"

namespace common {

    Location::Location() { }

    /*
    Location::Location(const common::Location& other) {
        lines = other.lines;
        columns = other.columns;
        source = other.source;
    }
    */

    Location::Location(int l, int c) {
        lines = l;
        columns = c;
    }

    ostream &operator<<(ostream &o, const Location &loc) {
        //if (loc.source.empty())
            return o << loc.lines << ":" << loc.columns;
        //return o << loc.source << ":" << loc.lines << ":" << loc.columns;
    }
}
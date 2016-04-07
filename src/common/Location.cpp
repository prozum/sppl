#include "Location.h"

namespace common {

    Location::Location() { }

    Location::Location(int l, int c, string src) {
        lines = l;
        columns = c;
        source = src;
    }

    ostream &operator<<(ostream &o, const Location &loc) {
        return o << loc.source << ":" << loc.lines << ":" << loc.columns;
    }
}
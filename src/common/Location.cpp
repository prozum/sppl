#include "Location.h"

namespace common {

    Location::Location() { }

    Location::Location(unsigned l, unsigned c, string src) :
            begin(l, c, src),
            end(l, c, src) { }

    void Location::step() {
        begin = end;
    }

    void Location::columns(unsigned count) {
        end.column = max(1u, end.column + count);
    }

    void Location::lines(unsigned count) {
        end.column = 1;
        end.line += count;
    }

    ostream &operator<<(ostream &out, const Location &loc) {
        if (loc.begin.source.empty())
            return out << loc.begin.line << ":" << loc.begin.column;
        return out << loc.begin.source << ":"
               << loc.begin.line << ":" << loc.begin.column;
    }
}
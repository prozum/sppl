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
        end.Column = max(1u, end.Column + count);
    }

    void Location::lines(unsigned count) {
        end.Column = 1;
        end.Line += count;
    }

    void Location::sources(string src) {
        end.Src = src;
    }

    Location::Location(const Location &other) {
        begin = other.begin;
        end = other.end;
    }

    ostream &operator<<(ostream &out, const Location &loc) {
        if (loc.begin.Src.empty())
            return out << loc.begin.Line << ":" << loc.begin.Column;
        return out << loc.begin.Src << ":"
               << loc.begin.Line << ":" << loc.begin.Column;
    }
}
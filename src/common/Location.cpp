#include "Location.h"

namespace common {

    Location::Location() { }

    Location::Location(size_t l, size_t c, std::string source) {
        this->l = l;
        this->c = c;
        this->source = source;
    }

    std::ostream &operator<<(ostream &o, const Location &loc) {
        return o << loc.source << ":" << loc.l << ":" << loc.c;
    }
}
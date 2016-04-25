#include "Location.h"

namespace common {

Location::Location() {}

Location::Location(unsigned Line, unsigned Column, string Src)
    : begin(Line, Column, Src), end(Line, Column, Src) {}

void Location::step() { begin = end; }

void Location::columns(unsigned Count) {
    end.Column = max(1u, end.Column + Count);
}

void Location::lines(unsigned Count) {
    end.Column = 1;
    end.Line += Count;
}

void Location::sources(string Src) { end.Src = Src; }

Location::Location(const Location &Other) {
    begin = Other.begin;
    end = Other.end;
}

ostream &operator<<(ostream &Out, const Location &Loc) {
    if (Loc.begin.Src.empty())
        return Out << Loc.begin.Line << ":" << Loc.begin.Column;
    return Out << Loc.begin.Src << ":" << Loc.begin.Line << ":"
               << Loc.begin.Column;
}
}
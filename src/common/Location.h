#pragma once

#include <memory>
#include <ostream>
#include <string>

namespace common {

struct Position {
    Position() = default;
    Position(unsigned Line, unsigned Column, std::string Src)
        : Line(Line), Column(Column), Src(Src) {}

    unsigned Line = 1;
    unsigned Column = 1;
    std::string Src;
};

class Location {
  public:
    Location();
    Location(unsigned Line, unsigned Column, std::string Src);
    Location(const Location &Other);

    void step();
    void columns(unsigned Count = 1);
    void lines(unsigned Count = 1);
    void sources(std::string Src);

    Position begin;
    Position end;
};

std::ostream &operator<<(std::ostream &Out, const Location &Loc);
}

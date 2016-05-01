#pragma once

#include "Common.h"
#include "Location.h"

#include <cstddef>
#include <string>

namespace common {
class Error {
  public:
    Error() = default;
    Error(std::string Msg);
    Error(std::string Msg, Location Loc);

    Error(const Error &Other);

    static Error NotImplemented(Location Loc);
    static Error Expected(std::string Msg, std::string Expected, std::string Actual,
                          Location Loc);
    static Error Binary(std::string Msg, common::BinaryOp &Op);
    static Error Unary(std::string Msg, common::UnaryOp &Op);

    std::string Msg;
    Location Loc;
};

std::ostream &operator<<(std::ostream &Out, const Error &Err);
}

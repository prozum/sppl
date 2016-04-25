#pragma once

#include "Common.h"
#include "Location.h"

#include <cstddef>
#include <string>

namespace common {
class Error {
  public:
    Error() = default;
    Error(string Msg);
    Error(string Msg, Location Loc);

    Error(const Error &Other);

    static Error NotImplemented(string Msg);
    static Error Expected(string Msg, string Expected, string Actual,
                          Location Loc);
    static Error Binary(string Msg, common::BinaryOp &Op);
    static Error Unary(string Msg, common::UnaryOp &Op);

    std::string Msg;
    Location Loc;
};

std::ostream &operator<<(ostream &Out, const Error &Err);
}

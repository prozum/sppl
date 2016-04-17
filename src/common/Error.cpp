#include "Error.h"

namespace common {
    Error::Error(std::string msg) : Msg(msg), Loc() { }
    Error::Error(std::string msg, common::Location loc) : Msg(msg), Loc(loc) { }

    std::ostream &operator<<(ostream& stream, const Error &err)
    {
        return (stream << err.Loc << "|" << err.Msg);
    }

    Error::Error(const Error &other) {
        Msg = other.Msg;
        Loc = other.Loc;
    }

    Error Error::NotImplemented(string msg) {
        return Error ("NotImplemented: " + msg);
    }

    Error Error::Expected(string msg, string expected, string actual, Location loc) {
        return Error(msg + ": Expected: '" + expected + "' Actual: '" + actual + "'", loc);
    }

    Error Error::Binary(string msg, common::BinaryOp &op) {
        return Error("'" + op.str() + "' " + msg + ": Left: '" + op.Left->str() + "' Right: '" + op.Right->str() + "'", op.Loc);
    }

    Error Error::Unary(string msg, common::UnaryOp &op) {
        return Error("'" + op.str() + "' " + msg + ": Child: '" + op.Child->str() + "'", op.Loc);
    }

}

#include "Error.h"

namespace common {
    Error::Error(std::string msg) : msg(msg), loc() { }
    Error::Error(std::string msg, common::Location loc) : msg(msg), loc(loc) { }

    std::ostream &operator<<(ostream& stream, const Error &err)
    {
        return (stream << err.loc << "|" << err.msg);
    }

    Error Error::NotImplemented(string msg) {
        return Error ("NotImplemented: " + msg);
    }

    Error Error::Expected(string msg, string expected, string actual, Location loc) {
        return Error(msg + ": Expected: '" + expected + "' Actual: '" + actual + "'", loc);
    }

    Error Error::Binary(string msg, common::BinaryOp &op) {
        return Error("'" + op.str() + "' " + msg + ": Left: '" + op.left->str() + "' Right: '" + op.right->str() + "'", op.loc);
    }

    Error Error::Unary(string msg, common::UnaryOp &op) {
        return Error("'" + op.str() + "' " + msg + ": Child: '" + op.child->str() + "'", op.loc);
    }
}

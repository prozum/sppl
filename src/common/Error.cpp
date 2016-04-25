#include "Error.h"

namespace common {
Error::Error(std::string Msg) : Msg(Msg), Loc() {}
Error::Error(std::string Msg, common::Location Loc) : Msg(Msg), Loc(Loc) {}

std::ostream &operator<<(ostream &Out, const Error &Err) {
    return (Out << Err.Loc << "|" << Err.Msg);
}

Error::Error(const Error &Other) {
    Msg = Other.Msg;
    Loc = Other.Loc;
}

Error Error::NotImplemented(string Msg) {
    return Error("NotImplemented: " + Msg);
}

Error Error::Expected(string Msg, string Expected, string Actual,
                      Location Loc) {
    return Error(
        Msg + ": Expected: '" + Expected + "' Actual: '" + Actual + "'", Loc);
}

Error Error::Binary(string Msg, common::BinaryOp &Op) {
    return Error("'" + Op.str() + "' " + Msg + ": Left: '" +
                     Op.Left->RetTy.str() + "' Right: '" +
                     Op.Right->RetTy.str() + "'",
                 Op.Loc);
}

Error Error::Unary(string Msg, common::UnaryOp &Op) {
    return Error("'" + Op.str() + "' " + Msg + ": Child: '" +
                     Op.Child->RetTy.str() + "'",
                 Op.Loc);
}
}

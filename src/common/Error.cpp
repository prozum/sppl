#include "Error.h"

namespace common {
    Error::Error(std::string msg, common::Location loc) : msg(msg), loc(loc) { }

    std::ostream &operator<<(ostream& stream, const Error &err)
    {
        return (stream << err.loc << "|" << err.msg);
    }
}

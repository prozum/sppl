#include "Variant.h"

namespace parser {
    Variant &Variant::operator=(const Variant &other) {
        memcpy(&ptr, &other.ptr, sizeof(Pointer));
        memcpy(&num, &other.num, sizeof(Numeric));
    }

    Variant::Variant() {
    }
}

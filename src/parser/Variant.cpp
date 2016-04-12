#include "Variant.h"

namespace parser {
    Variant &Variant::operator=(const Variant &other) {
        memcpy(&val, &other.val, sizeof(value));
    }

    Variant::Variant() {
    }
}

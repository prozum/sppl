#include "Variant.h"

namespace parser {

    Variant &Variant::operator=(const Variant &other) {
        memcpy(this, &other, sizeof(Variant));
        //memcpy(this, &other, sizeof(Numeric));
    }

    /*
    Variant::Variant() {
    }
     */
}

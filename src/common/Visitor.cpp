#include "Visitor.h"

using namespace std;

namespace common {
    void Visitor::addError(Error err) {
        Errors.push_back(err);
    }

    bool Visitor::hasError() {
        return Errors.size() != 0;
    }
}


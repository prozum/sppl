#include "Visitor.h"

using namespace std;

namespace common {
    void Visitor::addError(Error err) {
        Errors.push_back(err);
        notsafe = true;
    }

    bool Visitor::hasError() {
        return Errors.size() != 0;
    }

    void Visitor::outError(ostream &out) {

        for (auto &err: Errors) {
            out << err << endl;
        }

        Errors.clear();
    }
}


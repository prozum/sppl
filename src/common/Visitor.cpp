#include "Visitor.h"

using namespace std;

namespace common {
    void Visitor::AddError(Error err) {
        Errors.push_back(err);
        notsafe = true;
    }

    bool Visitor::HasError() {
        return Errors.size() != 0;
    }

    void Visitor::OutError(ostream &out) {

        for (auto &err: Errors) {
            out << err;
        }
    }
}


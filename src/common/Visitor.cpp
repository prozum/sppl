#include "Visitor.h"

using namespace std;
using namespace common;

void Visitor::addError(Error Err) { Errors.push_back(Err); }
void Visitor::clearErrors() {
    ExpectedErrors = 0;
    Errors.clear();
}
bool Visitor::checkErrors() {
    if (Errors.size() > ExpectedErrors) {
        ExpectedErrors = Errors.size();
        return true;
    }
    return false;
}

size_t Visitor::countErrors() {
    return Errors.size();
}

std::vector<Error> Visitor::getErrors() {
    return Errors;
}



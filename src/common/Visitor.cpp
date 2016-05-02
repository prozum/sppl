#include "Visitor.h"

using namespace std;
using namespace common;

void Visitor::addError(Error Err) {
    Safe = false;
    Errors.push_back(Err);
}
void Visitor::clearErrors() {
    Safe = true;
    Errors.clear();
}

size_t Visitor::countErrors() {
    return Errors.size();
}

bool Visitor::checkNotSafe() {
    return !Safe;
}

void Visitor::setSafe() {
    Safe = true;
}

std::vector<Error> Visitor::getErrors() {
    return Errors;
}



#include "Visitor.h"

using namespace std;
using namespace common;

void Visitor::addError(Error Err) { Errors.push_back(Err); }

bool Visitor::hasError() { return Errors.size() != 0; }

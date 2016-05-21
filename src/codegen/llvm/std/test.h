#ifndef SPPL_TEST_H
#define SPPL_TEST_H

#include "std.h"
#include "stdio.h"

SPPL_DECL void hello_world();
SPPL_DECL void secret(long guess);
SPPL_DECL double many_args(long arg1, double arg2, double arg3, double arg4);

#endif //SPPL_TEST_H

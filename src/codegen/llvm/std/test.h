#ifndef SPPL_TEST_H
#define SPPL_TEST_H

#include "std.h"
#include "stdio.h"

type_t hello_world_t[] = { { VOID, }, 0 };
SPPL_DECL(hello_world_t) void hello_world();
SPPL_DECL(hello_world_t) void secret(long guess);
SPPL_DECL(hello_world_t) double many_args(long arg1, double arg2, double arg3, double arg4);

#endif //SPPL_TEST_H

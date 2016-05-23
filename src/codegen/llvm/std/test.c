#include "test.h"

void hello_world() {
    printf("Hello World!");
}

void secret(long guess) {
    if (guess == 42)
        printf("A winner is you!");
    else
        printf("All your base are belong to us!");
}

double many_args(long arg1, double arg2, double arg3, double arg4) {
    return arg1 + arg2 + arg3 + arg4;
}

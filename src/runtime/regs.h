#ifndef SPPL_REGS_H
#define SPPL_REGS_H

#include <stdint.h>

typedef struct context_s {
    uint64_t rdi; // destination register
    uint64_t rsi; // source register
    uint64_t rbx; // base (not base pointer)
    uint64_t rbp; // base pointer
} context_t;

#endif //SPPL_REGS_H

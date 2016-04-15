#ifndef SPPL_REGS_H
#define SPPL_REGS_H

#include <stdint.h>

typedef struct context_s {
    uint64_t rdi; // destination register
    uint64_t rsi; // source register
    uint64_t rbx; // base (not base pointer)
    uint64_t rbp; // base pointer
} context_t;

void swtch_context(context_t **old, context_t *new);

void push_context();

void pop_context();

void save_scheduler_context(context_t *context);

#endif //SPPL_REGS_H

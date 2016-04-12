#ifndef SPPL_REGS_H
#define SPPL_REGS_H

typedef struct reg_table_s {
    // general purpose
    int64_t RAX;
    int64_t RBX;
    int64_t RCX;
    int64_t RDX;
    int64_t RSI;
    int64_t RDI;
    int64_t RBP; // base pointer
    int64_t RSP; // stack pointer
    int64_t R8;
    int64_t R9;
    int64_t R10;
    int64_t R11;
    int64_t R12;
    int64_t R13;
    int64_t R14;
    int64_t R15;

    // flags and instruction pointer
    int64_t RIP_RA; // instruction pointer
    int16_t flags; // only 12 bits are used

    // segment registers
    // apparently not used anymore
    //int64_t CS;
    //int64_t FS;
    //int64_t GS;
} reg_table_t;

/*
#define SAVE_CONTEXT()          \
__asm__ __volatile__(           \
    "push %%rax \n\t"           \
    "push %%rax \n\t"           \
    "push %%rbx \n\t"           \
    "push %%rcx \n\t"           \
    "push %%rdx \n\t"           \
    "push %%rsi \n\t"           \
    "push %%rdi \n\t"           \
    "push %%rbp \n\t"           \
    "push %%rsp \n\t"           \
    "push %%r8 \n\t"            \
    "push %%r9 \n\t"            \
    "push %%r10 \n\t"           \
    "push %%r11 \n\t"           \
    "push %%r12 \n\t"           \
    "push %%r13 \n\t"           \
    "push %%r14 \n\t"           \
    "push %%r15 \n\t"           \
    "push %%rflags \n\t"        \
    "mov %%rsp %%rax \n\t"      \
);

#define RESTORE_CONTEXT()       \
__asm__ __volatile__(           \
    "pop %%rax \n\t"           \
    "pop %%rax \n\t"           \
    "pop %%rbx \n\t"           \
    "pop %%rcx \n\t"           \
    "pop %%rdx \n\t"           \
    "pop %%rsi \n\t"           \
    "pop %%rdi \n\t"           \
    "pop %%rbp \n\t"           \
    "pop %%rsp \n\t"           \
    "pop %%r8 \n\t"            \
    "pop %%r9 \n\t"            \
    "pop %%r10 \n\t"           \
    "pop %%r11 \n\t"           \
    "pop %%r12 \n\t"           \
    "pop %%r13 \n\t"           \
    "pop %%r14 \n\t"           \
    "pop %%r15 \n\t"           \
    "pop %%rflags \n\t"        \
);
*/

#endif //SPPL_REGS_H

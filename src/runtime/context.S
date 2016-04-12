.globl push_context
push_context:
    pushq    %rax
    pushq    %rcx
    pushq    %rdx
    pushq    %r8
    pushq    %r9
    pushq    %r10
    pushq    %r11
    pushq    %r12
    pushq    %r13
    pushq    %r14
    pushq    %r15

.globl pop_context
pop_context:
    popq    %rax
    popq    %rcx
    popq    %rdx
    popq    %r8
    popq    %r9
    popq    %r10
    popq    %r11
    popq    %r12
    popq    %r13
    popq    %r14
    popq    %r15

.globl swtch_context
swtch_context:
    movq    8(%rsp), %rax
    movq    16(%rsp), %rdx

    pushq   %rbp
    pushq   %rbx
    pushq   %rsi
    pushq   %rdi

    movq    %rsp, (%rax)
    movq    %rdx, rsp

    popq    %rdi
    popq    %rsi
    popq    %rbx
    popq    %rbp
    ret

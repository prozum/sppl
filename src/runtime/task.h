#ifndef SPPL_TASK_H
#define SPPL_TASK_H

#include <pthread.h>
#include <stdint.h>
#include <setjmp.h>

#include "regs.h"

typedef struct task_s task_t;
typedef void (*func)(task_t *);

typedef enum state_e {
    NEW,
    WAITING,
    ACTIVE,
    DONE,
    DEAD
} state_t;

typedef struct task_s {
    int64_t id;
    reg_table_t regs;
    state_t state;
    void *args;
    func f;
    int sub_task_len;
    task_t **sub_tasks;
} tast_t;

#endif //SPPL_TASK_H

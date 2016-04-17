#include "regs.h"
#include "runtime.h"
#include "queue.h"

#include <stdio.h>

typedef struct int_s {
    int input;
    int res;
} int_t;

void fun1 (void *t) {
    ((int_t *)t)->res = 1;
}

void fun2 (void *t) {
    ((int_t *)t)->res = 1;
}

void fun3 (void *t) {

    task_t *task = (task_t *)t;

    int_t arg1;
    int_t arg2;

    task_t task1;
    task1.f = fun1;
    task1.args = (void *)(&arg1);

    task_t task2;
    task2.f = fun2;
    task2.args = (void *)(&arg2);

    task->sub_tasks[0] = &task1;
    task->sub_tasks[1] = &task2;

    add_task(&task1);
    add_task(&task2);

    switch_task(task);

    ((int_t *) t)->res = arg1.res + arg2.res;
}

int main()
{
    task_t *task;
    int_t args;

    task->args = (void *)(&args);
    task->f = fun3;

    task->sub_tasks = malloc(sizeof(task_t *) * 2);

    rmain(1, task);

    printf("1 - %d\n", args.res);

    return 0;
}
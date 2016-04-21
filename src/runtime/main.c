#include "task.h"

typedef struct int_s {
    int input;
    int res;
} int_t;

void fun1 (void *t) {
    printf("fun1\n");

    task_t *task = (task_t *)t;

    ((int_t *)(task->startarg))->res = 1;

    printf("fun1 res: %d\n",  ((int_t *)(task->startarg))->res);
}

void fun2 (void *t) {
    printf("fun2\n");

    task_t *task = (task_t *)t;

    ((int_t *)(task->startarg))->res = 1;
}

void fun3 (void *t) {

    task_t *task = (task_t *)t;
    int_t *arg = (int_t *)task->startarg;

    int_t *arg1 = malloc(sizeof(int_t));
    int_t *arg2 = malloc(sizeof(int_t));

    arg1->res = 1;

    task_t *task1 = taskcreate(fun1, (void *)arg1, 0);
    task_t *task2 = taskcreate(fun2, (void *)arg2, 0);

    task->sub_tasks[0] = task1;
    task->sub_tasks[1] = task2;

    taskadd(task2);
    taskadd(task1);

    printf("fun3\n");

    taskyield(task);

    printf("fun1 res: %d\n", arg1->res);

    arg1->res = 1;

    printf("fun1 res: %d\n", arg1->res);

    printf("%d, %d\n", arg1->res, arg2->res);

    arg->res = arg1->res + arg2->res;

    /*
    free(task->sub_tasks);
    free(arg1);
    free(arg2);
    free(task1);
    free(task2);
     */
}

int main() {

    int_t *arg = malloc(sizeof(int_t) * 1);

    task_t *task = taskcreate(fun3, arg, 2);

    rmain(2, task);

    return 0;
}
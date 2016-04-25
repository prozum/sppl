#include "task.h"

typedef struct int_s {
    int input;
    int res;
} int_t;

void fun2 (task_t *t) {
    printf("fun2\n");

    ((int_t *)(t->startarg))->res = 1;

    taskexit(t);
}

void fun1 (task_t *t) {
    printf("fun1\n");

    ((int_t *)(t->startarg))->res = 1;

    taskexit(t);
}

void fun3 (task_t *t) {
    printf("fun3\n");

    task_t *task = (task_t *)t;
    int_t *arg = (int_t *)task->startarg;

    int_t *arg1 = malloc(sizeof(int_t));
    int_t *arg2 = malloc(sizeof(int_t));

    task_t *task1 = taskcreate((void *)&fun1, (void *)arg1, 0);
    task_t *task2 = taskcreate((void *)&fun2, (void *)arg2, 0);

    task->sub_tasks[0] = task1;
    task->sub_tasks[1] = task2;

    taskadd(task1);
    taskadd(task2);

    printf("added tasks\n");

    taskyield(task);

    arg->res = arg1->res + arg2->res;

    printf("res: %d\n", arg->res);

    /*
    free(task->sub_tasks);
    free(arg1);
    free(arg2);
    free(task1);
    free(task2);
    */

    taskexit(task);
}

int main() {

    int_t *arg = malloc(sizeof(int_t));

    task_t *task = taskcreate((void *)&fun3, arg, 2);

    rmain(2, task);

    return 0;
}
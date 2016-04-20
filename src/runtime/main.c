#include "runtime.h"
#include "task.h"

#include <stdio.h>

/*
typedef struct int_s {
    int input;
    int res;
} int_t;

void fun1 (void *t) {
    printf("fun1\n");

    task_t *task = (task_t *)t;

    ((int_t *)(task->args))->res = 1;

    printf("fun1 res: %d\n",  ((int_t *)(task->args))->res);
}

void fun2 (void *t) {
    printf("fun2\n");

    task_t *task = (task_t *)t;

    ((int_t *)(task->args))->res = 1;
}

void fun3 (void *t) {

    task_t *task = (task_t *)t;
    int_t *arg = (int_t *)task->args;

    task->sub_task_len = 2;
    task->sub_tasks = malloc(sizeof(task_t) * 2);

    int_t *arg1 = malloc(sizeof(int_t));
    int_t *arg2 = malloc(sizeof(int_t));

    arg1->res = 1;

    task_t *task1 = malloc(sizeof(task_t));
    task1->f = fun1;
    task1->args = (void *)(arg1);

    task_t *task2 = malloc(sizeof(task_t));
    task2->f = fun2;
    task2->args = (void *)(arg2);

    task->sub_tasks[0] = task1;
    task->sub_tasks[1] = task2;

    add_task(task2);
    add_task(task1);

    printf("fun3\n");

    yield_waiting(task);

    printf("fun1 res: %d\n", arg1->res);

    arg1->res = 1;

    printf("fun1 res: %d\n", arg1->res);

    printf("%d, %d\n", arg1->res, arg2->res);

    arg->res = arg1->res + arg2->res;

    free(task->sub_tasks);
    free(arg1);
    free(arg2);
    free(task1);
    free(task2);
}
*/

/*
int main()
{
    task_t *task = malloc(sizeof(task_t));
    int_t *args = malloc(sizeof(int_t));

    task->args = (void *)(args);
    task->f = fun3;

    rmain(1, task);

    printf("result: %d\n", args->res);

    free(task);
    free(args);

    return 0;
}
 */

void
        taskmain(int argc, char **argv) {
    
}
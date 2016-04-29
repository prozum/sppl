#include "src/runtime/task.h"

typedef struct sig1 {
    void (*par)(task_t*);
    int (*seq)(task_t*, int);
} sig1;

typedef struct int_s {
    int input;
    int res;
} int_t;

void superadd_par(task_t *t);
int superadd_seq(task_t *t, int a);
sig1 superadd = { superadd_par, superadd_seq };

void superadd_par(task_t *t) {
    int_t *arg = (int_t*)t->startarg;

    if (arg->input == 0) {
        arg->res = 1;

        taskexit(t);
    }

    {
        int_t *arg1 = malloc(sizeof(int_t));
        arg1->input = arg->input - 1;

        task_t *task1 = taskcreate((void (*)(void*))superadd.par, (void*)arg1);

        subtaskadd(t, task1);
        int res2 = superadd.seq(t, arg->input - 1);

        taskyield(t);

        arg->res = arg1->res + res2;

        taskexit(t);
    }
}

int superadd_seq(task_t *t, int a) {

    if (a == 0) {
        return 1;
    }

    {
        int_t *arg1 = malloc(sizeof(int_t));
        arg1->input = a - 1;

        task_t *task1 = taskcreate((void (*)(void*))superadd.par, (void*)arg1);

        subtaskadd(t, task1);
        int res2 = superadd.seq(t, a - 1);

        taskyield(t);

        return arg1->res + res2;
    }
}

int main() {
    int_t *arg = malloc(sizeof(int_t));
    arg->input = 14;

    task_t *task = taskcreate((void (*)(void*))superadd.par, arg);

    rmain(4, task);

    printf("res: %d\n", arg->res);

    taskdealloc(task);

    return 0;
}
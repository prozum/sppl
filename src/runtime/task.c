#include "task.h"

uint        placeholder_stack_size = 256 * 2048;

task_t*
taskalloc(void (*fn)(void*), void *arg, uint stack, uint sub_tasks)
{
	task_t *t;

    t = malloc(sizeof(*t));

	t->stk = (uchar*)(t+1);
	t->stksize = stack;
	t->startfn = fn;
	t->startarg = arg;
	t->sub_task_len = sub_tasks;
    t->sub_tasks = malloc(sizeof(task_t *) * sub_tasks);
    t->state = NEW;

	if(getcontext(&t->context.uc) < 0){
		fprint(2, "getcontext: %r\n");
		abort();
	}

    getcontext(&t->context.uc);

    t->context.uc.uc_link = 0;
    t->context.uc.uc_stack.ss_sp = malloc(stack);
    t->context.uc.uc_stack.ss_size = stack;
    t->context.uc.uc_stack.ss_flags = 0;

	makecontext(&t->context.uc, (void *)t->startfn, 1, t);

	return t;
}

task_t *
taskcreate(void (*fn)(void*), void *arg, uint sub_tasks)
{
    return taskalloc(fn, arg, placeholder_stack_size, sub_tasks);
}

void
taskadd(task_t *t)
{
    queue_head *head = malloc(sizeof(queue_head));
    head->item = (void *)t;

    queue_add(head, runtime.queue);
}

void
taskswitch(task_t *t)
{
    needstack(t, 0);
	contextswitch(&t->context, runtime.scheduler_pool[t->scheduler_id].context);
}

void
taskyield(task_t *t)
{
    t->state = WAITING;
    taskadd(t);
	taskswitch(t);
}

void
taskexit(task_t *t)
{
    t->state = DONE;
	taskswitch(t);
}

void
contextswitch(context_t *from, context_t *to)
{
	if(swapcontext(&from->uc, &to->uc) < 0){
		fprint(2, "swapcontext failed: %r\n");
		assert(0);
	}
}

void
needstack(task_t *t, int n)
{
	if((char*)&t <= (char*)t->stk
	|| (char*)&t - (char*)t->stk < 256+n){
		fprint(2, "task stack overflow: &t=%p tstk=%p n=%d\n", &t, t->stk, 256+n);
		abort();
	}
}



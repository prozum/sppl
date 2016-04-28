#include "task.h"

uint placeholder_stack_size = 64 * 1024/*254 * 2024*/;

task_t*
taskalloc(void (*fn)(void*), void *arg, uint stack)
{
	task_t *t;

    t = malloc(sizeof(*t));

	t->stk = (uchar*)(t+1);
	t->startfn = fn;
	t->startarg = arg;
	t->sub_task_len = 0;
	t->sub_tasks_alloc = 0;
    //t->sub_tasks = malloc(sizeof(task_t *) * sub_tasks);
    t->state = NEW;

	if(getcontext(&t->context.uc) < 0){
		fprint(2, "getcontext: %r\n");
		abort();
	}

    getcontext(&t->context.uc);

    // allocate stack and all the shazzlebang for the task
    t->context.uc.uc_link = 0;
    t->context.uc.uc_stack.ss_sp = malloc(stack);
    t->context.uc.uc_stack.ss_size = stack;
    t->context.uc.uc_stack.ss_flags = 0;

	makecontext(&t->context.uc, (void *)t->startfn, 1, t);

	return t;
}

void
taskdealloc(task_t *t)
{
	free(t->context.uc.uc_stack.ss_sp);
    if (t->sub_tasks_alloc) {
        free(t->sub_tasks);
    }
	free(t->startarg);
	free(t);
}

task_t *
taskcreate(void (*fn)(void*), void *arg)
{
    return taskalloc(fn, arg, placeholder_stack_size);
}

void
taskadd(task_t *t)
{
    queue_head *head = malloc(sizeof(queue_head));
    head->item = (void *)t;

    queue_add(head, runtime.queue);
}

void
subtaskadd(task_t *parent, task_t *subtask)
{
	if (parent->sub_task_len == 0) {
		parent->sub_tasks = malloc(sizeof(task_t *));
		parent->sub_task_len++;
		parent->sub_tasks_alloc++;
		parent->sub_tasks[0] = subtask;
	} else if (parent->sub_task_len == parent->sub_tasks_alloc) {
		parent->sub_tasks = realloc(parent->sub_tasks, sizeof(task_t *) * parent->sub_tasks_alloc * 2);
		parent->sub_tasks[parent->sub_task_len] = subtask;
		parent->sub_task_len++;
		parent->sub_tasks_alloc *= 2;
	} else {
		parent->sub_tasks[parent->sub_task_len] = subtask;
		parent->sub_task_len++;
	}

	taskadd(subtask);
}

void
taskswitch(task_t *t)
{
    //needstack(t, 0);
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



#include "task.h"

uint        placeholder_stack_size = 256 * 1000;

static void
taskstart(uint y, uint x)
{
	task_t *t;
	ulong z;

	z = x<<16;	//hide undefined 32-bit shift from 32-bit compilers
	z <<= 16;
	z |= y;
	t = (task_t*)z;

	t->startfn((void *)t);
	//taskexit(t);
}


task_t*
taskalloc(void (*fn)(void*), void *arg, uint stack, uint sub_tasks)
{
	task_t *t;
	sigset_t zero;
	uint x, y;
	ulong z;

	/* allocate the task and stack together */
	t = malloc(sizeof *t+stack);

	memset(t, 0, sizeof *t);
	t->stk = (uchar*)(t+1);
	t->stksize = stack;
	t->startfn = fn;
	t->startarg = arg;
	t->sub_task_len = sub_tasks;
    t->sub_tasks = malloc(sizeof(task_t *) * sub_tasks);
    t->state = NEW;

	/* do a reasonable initialization */
	memset(&t->context.uc, 0, sizeof(t->context.uc));
	sigemptyset(&zero);
	sigprocmask(SIG_BLOCK, &zero, &t->context.uc.uc_sigmask);

	/* must initialize with current context */
	if(getcontext(&t->context.uc) < 0){
		fprint(2, "getcontext: %r\n");
		abort();
	}

	/* call makecontext to do the real work. */
	/* leave a few words open on both ends */
	t->context.uc.uc_stack.ss_sp = t->stk+8;
	t->context.uc.uc_stack.ss_size = t->stksize-64;

	/*
	 * All this magic is because you have to pass makecontext
	 * function that takes some number of word-sized variables,
	 * and on 64-bit machines pointers are bigger than words.
	 */

	z = (ulong)t;
	y = z;
	z >>= 16;	//hide undefined 32-bit shift from 32-bit compilers
	x = z>>16;
	makecontext(&t->context.uc, (void(*)())taskstart, 2, y, x);

	return t;
}

task_t *
taskcreate(void (*fn)(void*), void *arg, uint sub_tasks)
{
    return taskalloc(fn, arg, placeholder_stack_size, sub_tasks);
}

/*
task_t *
taskcreate(void (*fn)(void*), void *arg, uint stack, uint sub_tasks)
{
	task_t *t;

	t = taskalloc(fn, arg, stack, sub_tasks);
    t->state = NEW;

	//t->ready = 1;

	queue_head *new = malloc(sizeof(queue_head));
	new->item = (void *)t;
	queue_add(new, runtime.queue);
}
 */


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
	//t->exiting = 1;
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



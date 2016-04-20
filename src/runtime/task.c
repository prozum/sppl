#include "task.h"

static void
taskstart(uint y, uint x)
{
	Task *t;
	ulong z;

	z = x<<16;	/* hide undefined 32-bit shift from 32-bit compilers */
	z <<= 16;
	z |= y;
	t = (Task*)z;

	t->startfn(t->startarg);
	taskexit(t);
}

static Task*
taskalloc(void (*fn)(void*), void *arg, uint stack)
{
	Task *t;
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

	/* do a reasonable initialization */
	memset(&t->context.uc, 0, sizeof t->context.uc);
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
	z >>= 16;	/* hide undefined 32-bit shift from 32-bit compilers */
	x = z>>16;
	makecontext(&t->context.uc, (void(*)())taskstart, 2, y, x);

	return t;
}

void
taskcreate(void (*fn)(void*), void *arg, uint stack)
{
	Task *t;

	t = taskalloc(fn, arg, stack);

	t->ready = 1;

	queue_head *new = malloc(sizeof(queue_head));
	new->item = (void *)t;
	queue_add(new, runtime.queue);
}


void
taskadd(Task *t)
{
    queue_head *head = malloc(sizeof(queue_head));
    head->item = (void *)t;

    queue_add(head, runtime.queue);
}

void
taskready(Task *t)
{
    t->ready = 1;
    taskadd(t);
}

void
taskswitch(Task *t)
{
    needstack(t, 0);
	contextswitch(&t->context, t->scheduler);
}

void
taskyield(Task *t)
{
	taskready(t);
	taskswitch(t);
}

void
taskexit(Task *t)
{
	t->exiting = 1;
	taskswitch(t);
}

void
contextswitch(Context *from, Context *to)
{
	if(swapcontext(&from->uc, &to->uc) < 0){
		fprint(2, "swapcontext failed: %r\n");
		assert(0);
	}
}

void
needstack(Task *t, int n)
{
	if((char*)&t <= (char*)t->stk
	|| (char*)&t - (char*)t->stk < 256+n){
		fprint(2, "task stack overflow: &t=%p tstk=%p n=%d\n", &t, t->stk, 256+n);
		abort();
	}
}



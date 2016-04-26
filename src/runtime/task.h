#ifndef __TASK_H__
#define __TASK_H__

#include "defs.h"

#if defined(__arm__)
int getmcontext(mcontext_t*);
void setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif

#if defined(__mips__)
#include "mips-ucontext.h"
int getmcontext(mcontext_t*);
void setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif

#define print task_print
#define fprint task_fprint
#define snprint task_snprint
#define seprint task_seprint
#define vprint task_vprint
#define vfprint task_vfprint
#define vsnprint task_vsnprint
#define vseprint task_vseprint
#define strecpy task_strecpy

// definition of a lot of print functions
int print(char*, ...);
int fprint(int, char*, ...);
char *snprint(char*, uint, char*, ...);
char *seprint(char*, char*, char*, ...);
int vprint(char*, va_list);
int vfprint(int, char*, va_list);
char *vsnprint(char*, uint, char*, va_list);
char *vseprint(char*, char*, char*, va_list);
char *strecpy(char*, char*, char*);

void		contextswitch(context_t *from, context_t *to);

// enum with states tasks can be in
typedef enum task_state_e {
    NEW,
    WAITING,
    RUNNING,
    DONE
} task_state_t;

// task block
typedef struct task_s
{
    context_t	 context;   // the context of the task
    task_state_t state;     // which state the task is in (waiting, running, new, etc)
    uint 	scheduler_id;   // the id of the scheduler executing the task right now
	uint 	sub_tasks_alloc;	// how many subtasks there are allocated for
    uint    sub_task_len;   // number of sub tasks
    struct  task_s  **sub_tasks; // array of subtask references
    uchar	*stk;                // legacy shit for checking if task is running out of stack
    void	(*startfn)(void*);   // the function that the task should run
    void	*startarg;           // arguments for the task's function
} task_t;

task_t* taskcreate(void (*fn)(void*), void *arg);               // create task without giving stacksize
task_t* taskalloc(void (*fn)(void*), void *arg, uint stack);    // create task while giving stacksize
void	taskdealloc(task_t*);
void	taskadd(task_t *t);     // add task to queue
void	taskexit(task_t *t);    // exit task and mark it as done
void	taskyield(task_t *t);   // give control back to scheduler
void	needstack(task_t *t, int); // legacy
void	taskswitch(task_t *t);     // switch context to scheduler
void 	subtaskadd(task_t *parent, task_t *subtask); // add subtask to both queue and

typedef enum scheduler_state_e {
    SLACKING,   // scheduler state if it's not working
    WORKING     // scheduler state when working
} scheduler_state_t;

typedef struct scheduler_s {
    uint64_t id;            // scheduler id
    context_t *context;     // context of the scheduler so tasks can give control back to scheduler
} scheduler_t;


typedef struct runtime_s {
    uint64_t os_thread_count;   // allocated os threads for the runtime
    pthread_t *thread_pool;     // array of pthreads
    scheduler_t *scheduler_pool;// array of the schedulers, each one runs on their own pthread

    queue_root *queue;          // queue with tasks

    pthread_mutex_t scheduler_status_lock;  // lock for making sure that only one scheduler can check scheduler states
    scheduler_state_t *scheduler_status;    // array for checking the state of schedulers
} runtime_t;

runtime_t   runtime;    // instantiating a global runtime

// the runtime main procedure which should be called,
// it takes the number of how many os threads that should run
// and an initial task
void rmain(uint64_t os_thread_count, task_t *initial);
// the procedure that each pthread will run
void start_scheduler(void *sched_ptr);
// set the state of a scheduler
void set_active_worker(uint64_t id, scheduler_state_t state);
// check if all schedulers are inactive
uint64_t get_active_workers();
// check if subtasks for a given task are done
int get_subtasks_done(task_t *t);

/*
 * Threaded I/O.
 */
int		fdread(int, void*, int);
int		fdread1(int, void*, int);	//always uses fdwait
int		fdwrite(int, void*, int);
void	fdwait(int, int);
int		fdnoblock(int);

void		fdtask(void*);

/*
 * Network dialing - sets non-blocking automatically
 */
enum
{
	UDP = 0,
	TCP = 1,
};

int		netannounce(int, char*, int);
int		netaccept(int, char*, int*);
int		netdial(int, char*, int);
int		netlookup(char*, uint32_t*); //blocks entire program!

#endif //__TASK_H__
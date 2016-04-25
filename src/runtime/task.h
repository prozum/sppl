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

int print(char*, ...);
int fprint(int, char*, ...);
char *snprint(char*, uint, char*, ...);
char *seprint(char*, char*, char*, ...);
int vprint(char*, va_list);
int vfprint(int, char*, va_list);
char *vsnprint(char*, uint, char*, va_list);
char *vseprint(char*, char*, char*, va_list);
char *strecpy(char*, char*, char*);

char        *argv0;
void		contextswitch(context_t *from, context_t *to);

typedef enum task_state_e {
    NEW,
    WAITING,
    RUNNING,
    DONE
} task_state_t;

typedef struct task_s
{
    context_t	 context;
    task_state_t state;
    uint 	scheduler_id;
    uint    sub_task_len;
    struct  task_s  **sub_tasks;
    uchar	*stk;
    uint	stksize;
    void	(*startfn)(void*);
    void	*startarg;
    void	*ret_data;
} task_t;

task_t* taskalloc(void (*fn)(void*), void *arg, uint stack, uint sub_tasks);
task_t* taskcreate(void (*fn)(void*), void *arg, uint sub_tasks);
void	taskadd(task_t *t);
void	taskexit(task_t *t);
void	taskyield(task_t *t);
void	needstack(task_t *t, int);
void	taskswitch(task_t *t);

typedef enum scheduler_state_e {
    SLACKING,
    WORKING
} scheduler_state_t;

typedef struct scheduler_s {
    uint64_t id;
    context_t *context;
} scheduler_t;

typedef struct runtime_s {
    uint64_t os_thread_count;
    pthread_t *thread_pool;
    scheduler_t *scheduler_pool;

    queue_root *queue;

    pthread_mutex_t scheduler_status_lock;
    scheduler_state_t *scheduler_status;
} runtime_t;

runtime_t   runtime;

void rmain(uint64_t os_thread_count, task_t *initial);
void start_scheduler(void *sched_ptr);
void set_active_worker(uint64_t id, scheduler_state_t state);
uint64_t get_active_workers();
int get_subtasks_done(task_t *t);

/*
struct Tasklist	//used internally
{
	task_t	*head;
	task_t	*tail;
};
 */

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
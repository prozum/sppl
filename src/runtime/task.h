#include "defs.h"
#include "queue.h"
#include "print.h"

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

typedef struct Task_s
{
    uint    sched_id;
    char	name[256];	// offset known to acid
    char	state[256];
    struct Task	*next;
    struct Task	*prev;
    struct Task	*allnext;
    struct Task	*allprev;
    Context	context;
    uvlong	alarmtime;
    uchar	*stk;
    uint	stksize;
    int	    exiting;
    int	    alltaskslot;
    int	    system;
    int	    ready;
    void	(*startfn)(void*);
    void	*startarg;
    void	*udata;
} Task;

int		anyready(void);
void	taskcreate(void (*fn)(void*), void *arg, uint stack, queue_root *queue)
void	taskexit(Task *t, Context *context);
void	taskmain(int argc, char *argv[]);
void	taskyield(Task *t, Context *context);
void**	taskdata(Task *t);
void	needstack(Task *t, int);
void	taskname(Task *t, char*, ...);
void	taskstate(char*, ...);
char*	taskgetname(Task *t);
char*	taskgetstate(void);
void	tasksystem(void);
unsigned int	taskdelay(unsigned int);
unsigned int	taskid(void);
void	taskready(Task*);
void	taskswitch(Task *t, Context *context);

void	addtask(queue_root*, Task*);

/*
struct Tasklist	//used internally
{
	Task	*head;
	Task	*tail;
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

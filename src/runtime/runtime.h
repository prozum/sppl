#ifndef SPPL_RUNTIME_H
#define SPPL_RUNTIME_H

#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "regs.h"

typedef struct task_s task_t;
typedef void (*func)(void *);

typedef enum task_state_e {
    NEW,
    WAITING,
    BLOCKED,
    ACTIVE,
    DONE,
} task_state_t;

typedef enum scheduler_state {
    SLACKING,
    WORKING
} scheduler_state_e;

struct task_s {
    uint64_t scheduler_id;
    context_t *_context;
    task_state_t _state;
    char done;
    void *args;
    func f;
    uint32_t sub_task_len;
    task_t **sub_tasks;
};

typedef struct scheduler_s {
    uint64_t id;
    context_t *context;
    task_t *curr_task;
    queue_root *queue;
    pthread_t *io_worker;
} scheduler_t;

//global variables
uint64_t g_count;
queue_root *queue;
pthread_t *thread_pool;
scheduler_t *scheduler_pool;
pthread_mutex_t *scheduler_states_lock;
scheduler_state_e *scheduler_states;

//task functions
void add_task(task_t *task);
int check_subtasks_done(task_t *task);
void switch_task(task_t *task);

//scheduler and worker functions
void rmain(uint64_t sched_count, task_t *initial_task);
void run_scheduler(scheduler_t *sched);
uint64_t get_active_workers();
void set_active_workers(uint64_t id, scheduler_state_e state);

#endif //SPPL_RUNTIME_H
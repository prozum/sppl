#ifndef SPPL_RUNTIME_H
#define SPPL_RUNTIME_H

#include <pthread.h>
#include "queue.h"

/*
typedef void (*func)(void *);

typedef enum task_state_e {
    NEW     = 0,
    WAITING = 1,
    ACTIVE  = 2,
    BLOCKED = 3,
    DONE    = 4,
} task_state_t;

typedef struct task_s {
    uint64_t scheduler_id;
    ucontext_t *context;
    task_state_t state;
    void *args;
    func f;
    uint32_t sub_task_len;
    struct task_s **sub_tasks;
} task_t;

void add_task(task_t *task);
int check_subtasks_done(task_t *task);
void yield_waiting(task_t *task);
void yield_blocked(task_t *task);
*/


typedef enum scheduler_state_e {
    SLACKING,
    WORKING
} scheduler_state_t;

typedef struct scheduler_s {
    uint64_t id;
} scheduler_t;

typedef struct runtime_s {
    uint64_t os_thread_count;
    pthread_t *thread_pool;
    scheduler_t *scheduler_pool;

    queue_root *queue;

    pthread_mutex_t scheduler_status_lock;
    scheduler_state_t *scheduler_status;
} runtime_t;


// global runtime
runtime_t runtime;

/*
void rmain(uint64_t sched_count, Task *initial);
void start_scheduler(void *sched_ptr);
void set_active_worker(uint64_t id, scheduler_state_t state);
uint64_t get_active_workers();
*/

#endif //SPPL_RUNTIME_H
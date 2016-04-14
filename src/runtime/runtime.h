#ifndef SPPL_RUNTIME_H
#define SPPL_RUNTIME_H

#include <pthread.h>

#include "queue.h"
#include "task.h"
#include "regs.h"

typedef struct worker_s {
    int64_t worker_id;
    pthread_t *thread;
    queue_root *queue;
} worker_t;

typedef struct scheduler_s {
    context_t context;
    worker_t *worker;
    pthread_t *io_worker;
} scheduler_t;

typedef struct migrator_s {
    scheduler_t *schedulers;
} migrator_t;

scheduler_t *create_scheduler(int64_t workers);
worker_t *create_worker(int64_t id);

#endif //SPPL_RUNTIME_H

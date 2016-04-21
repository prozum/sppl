#include "task.h"

const uint  start_stack_size = 256*1024;

//runtime entry point
void rmain(uint64_t os_thread_count, task_t *initial) {
    runtime.os_thread_count = os_thread_count;
    runtime.queue = create_queue();
    runtime.scheduler_pool = malloc(sizeof(scheduler_t) * os_thread_count);
    runtime.thread_pool = malloc(sizeof(pthread_t) * os_thread_count);
    runtime.scheduler_status = malloc(sizeof(scheduler_state_t) * os_thread_count);
    pthread_mutex_init(&runtime.scheduler_status_lock, NULL);

    queue_head m;
    m.item = (void *)initial;

    printf("Shit actually happens!\n");

    queue_add(&m, runtime.queue);

    for (uint64_t i = 0; i < os_thread_count; ++i) {
        runtime.scheduler_pool[i].id = i;
        pthread_create(&runtime.thread_pool[i], NULL, (void *)start_scheduler, &runtime.scheduler_pool[i]);
    }

    for (uint64_t j = 0; j < os_thread_count; ++j) {
        pthread_join(runtime.thread_pool[j], NULL);
    }

    free(runtime.scheduler_pool);
    free(runtime.thread_pool);
    free(runtime.scheduler_status);

    pthread_mutex_destroy(&runtime.scheduler_status_lock);
}


void start_scheduler(void *sched_ptr) {
    scheduler_t *scheduler = (scheduler_t *) sched_ptr;
    task_t *t;
    scheduler->context = malloc(sizeof(context_t));

    set_active_worker(scheduler->id, WORKING);

    printf("Shit actually happens p2!\n");

    do {
        t = queue_get(runtime.queue)->item;

        if (get_active_workers() == 0) {
            break;
        } else if (t == nil) {
            set_active_worker(scheduler->id, SLACKING);
        } else {
            set_active_worker(scheduler->id, WORKING);
            t->state = RUNNING;
            contextswitch(scheduler->context, &t->context);
        }
    } while (1);

    pthread_exit(NULL);
}

uint64_t get_active_workers() {
    pthread_mutex_lock(&runtime.scheduler_status_lock);

    uint64_t acc = 0;

    for (uint64_t i = 0; i < runtime.os_thread_count; ++i) {
        acc += runtime.scheduler_status[i];
    }

    pthread_mutex_unlock(&runtime.scheduler_status_lock);

    return acc;
}

void set_active_worker(uint64_t id, scheduler_state_t state) {
    pthread_mutex_lock(&runtime.scheduler_status_lock);

    runtime.scheduler_status[id] = state;

    pthread_mutex_unlock(&runtime.scheduler_status_lock);
}

int get_subtasks_done(task_t *t) {

    for (int i = 0; i < t->sub_task_len; ++i) {
        if (t->sub_tasks[i]->state != DONE) {
            return 0;
        }
    }

    return 1;
}
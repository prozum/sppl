#include "task.h"

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

    queue_add(&m, runtime.queue);

    uint64_t i, j;

    for (i = 0; i < os_thread_count; ++i) {
        runtime.scheduler_pool[i].id = i;
        pthread_create(&runtime.thread_pool[i], NULL, (void *)start_scheduler, &runtime.scheduler_pool[i]);
    }

    for (j = 0; j < os_thread_count; ++j) {
        pthread_join(runtime.thread_pool[j], NULL);
    }

    free(runtime.scheduler_pool);
    free(runtime.thread_pool);
    free(runtime.scheduler_status);
    free(runtime.queue);

    pthread_mutex_destroy(&runtime.scheduler_status_lock);
}


void start_scheduler(void *sched_ptr) {
    scheduler_t *scheduler = (scheduler_t *) sched_ptr;
    task_t *t;
    queue_head *head;

    scheduler->context = malloc(sizeof(context_t));

    set_active_worker(scheduler->id, WORKING);

    do {
        head = queue_get(runtime.queue);

        if (head != nil) {
            t = (task_t *)head->item;
            if (t->state == NEW) {
                set_active_worker(scheduler->id, WORKING);
                t->state = RUNNING;
                t->scheduler_id = (uint)scheduler->id;
                contextswitch(scheduler->context, &t->context);
            } else if (t->state == WAITING && get_subtasks_done(t) != 0) {
                set_active_worker(scheduler->id, WORKING);
                t->state = RUNNING;
                t->scheduler_id = (uint)scheduler->id;
                contextswitch(scheduler->context, &t->context);
            } else {
                queue_add(head, runtime.queue);
            }
        } else {
            set_active_worker(scheduler->id, SLACKING);
        }

        if (get_active_workers() == 0) {
            break;
        }
    } while (1);

    free(scheduler->context);

    pthread_exit(NULL);
}

uint64_t get_active_workers() {
    pthread_mutex_lock(&runtime.scheduler_status_lock);

    uint64_t i, acc = 0;

    for (i = 0; i < runtime.os_thread_count; ++i) {
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

    int i;

    for (i = 0; i < t->sub_task_len; ++i) {
        if (t->sub_tasks[i]->state != DONE) {
            return 0;
        }
    }

    return 1;
}
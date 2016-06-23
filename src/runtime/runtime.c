#include "task.h"

//runtime entry point
void rmain(uint64_t os_thread_count, task_t *initial) {

    initial->initial = 1;

    runtime.os_thread_count = os_thread_count;
    runtime.queue = create_queue();
    runtime.initial_status = 0;
    runtime.scheduler_pool = malloc(sizeof(scheduler_t) * os_thread_count);
    runtime.thread_pool = malloc(sizeof(pthread_t) * os_thread_count);

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
    free(runtime.queue);
}


void start_scheduler(void *sched_ptr) {
    scheduler_t *scheduler = (scheduler_t *) sched_ptr;
    task_t *t;
    queue_head *head;

    scheduler->context = malloc(sizeof(context_t));

    do {
        head = queue_get(runtime.queue);

        if (head != nil) {
            t = (task_t *)head->item;
            if (t->state == NEW) {
                t->state = RUNNING;
                t->scheduler_id = (uint)scheduler->id;
                contextswitch(scheduler->context, &t->context);
            } else if (t->state == WAITING && get_subtasks_done(t) != 0) {
                t->state = RUNNING;
                t->scheduler_id = (uint)scheduler->id;
                contextswitch(scheduler->context, &t->context);
            } else {
                queue_add(head, runtime.queue);
            }
        } else if (runtime.initial_status) {
            break;
        }
    } while (1);

    free(scheduler->context);

    pthread_exit(NULL);
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

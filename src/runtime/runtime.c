#include "runtime.h"
#include <stdio.h>

void add_task(task_t *task) {

    task->state = NEW;

    queue_head *head = malloc(sizeof(queue_head));
    head->item = task;

    queue_add(head, runtime.queue);
}

int check_subtasks_done(task_t *task) {

    printf("checking subtasks\n");

    for (uint32_t i = 0; i < task->sub_task_len; ++i) {

        if (task->state != DONE) {
            return 0;
        }
    }

    return 1;
}

void yield(task_t *task) {
    task->state = WAITING;

    queue_head *head = malloc(sizeof(queue_head));
    head->item = task;
    queue_add(head, runtime.queue);

    task->context = malloc(sizeof(ucontext_t));

    printf("entered yield - %lu\n", task->scheduler_id);

    swapcontext(task->context, runtime.scheduler_pool[task->scheduler_id].context);
}

//runtime entry point
void rmain(uint64_t os_thread_count, task_t *initial_task) {
    runtime.os_thread_count = os_thread_count;
    runtime.queue = create_queue();
    runtime.scheduler_pool = malloc(sizeof(scheduler_t) * os_thread_count);
    runtime.thread_pool = malloc(sizeof(pthread_t) * os_thread_count);
    runtime.scheduler_status = malloc(sizeof(scheduler_state_t) * os_thread_count);
    pthread_mutex_init(&runtime.scheduler_status_lock, NULL);

    add_task(initial_task);

    for (uint64_t i = 0; i < os_thread_count; ++i) {
        runtime.scheduler_pool[i].id = i;
        runtime.scheduler_pool[i].io_worker = malloc(sizeof(pthread_t));
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

    scheduler_t *scheduler = (scheduler_t *)sched_ptr;

    scheduler->context = malloc(sizeof(ucontext_t));

    queue_head *curr;
    task_t *curr_task;

    do {
        getcontext(scheduler->context);

        curr = queue_get(runtime.queue);

        printf("inside run_scheduler do-while\n");

        if (curr != NULL) {

            curr_task = (task_t *)(curr->item);
            curr_task->scheduler_id = scheduler->id;
            set_active_worker(scheduler->id, WORKING);
            if ((curr_task->state == WAITING) && check_subtasks_done(curr_task)) {
                scheduler->curr_task = curr_task;
                swapcontext(scheduler->context, curr_task->context);
            } else if (curr_task->state == NEW) {
                printf("inside run_scheduler NEW\n");
                scheduler->curr_task = curr_task;
                scheduler->curr_task->state = ACTIVE;
                curr_task->f(curr_task->args);
                scheduler->curr_task->state = DONE;
            } else {
                queue_add(curr, runtime.queue);
            }
        } else if (get_active_workers() == 0) {
            break;
        } else {
            set_active_worker(scheduler->id, SLACKING);
        }
    } while (1);

    printf("inside run_scheduler about to free context\n");

    free(scheduler->context);

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
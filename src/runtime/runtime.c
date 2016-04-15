#include "runtime.h"

void add_task(task_t *task) {

    task->_state = NEW;
    task->done = 0;

    queue_head *head = malloc(sizeof(queue_head));
    head->item = task;

    queue_add(head, queue);
}

int check_subtasks_done(task_t *task) {

    uint32_t acc = 0;

    for (uint32_t i = 0; i < task->sub_task_len; ++i) {
        acc += task->done;
    }

    return acc == task->sub_task_len;
}

void switch_task(task_t *task) {
    task->_state = WAITING;

    queue_head *head = malloc(sizeof(queue_head));
    head->item = task;
    queue_add(head, queue);

    push_context();

    swtch_context(&(task->_context), scheduler_pool[task->scheduler_id].context);

    pop_context();
}

//runtime entry point
void rmain(uint64_t count, task_t *initial_task) {

    // initialize pool of schedulers/workers and threads
    scheduler_pool = malloc(sizeof(scheduler_t) * count);
    thread_pool = malloc(sizeof(pthread_t) * count);

    // initialize global variables
    queue = create_queue();
    g_count = count;
    pthread_mutex_init(scheduler_states_lock, NULL);
    scheduler_states = malloc(sizeof(scheduler_state_e) * count);

    add_task(initial_task);

    for (uint64_t i = 0; i < count; ++i) {
        scheduler_pool[i].id = i;
        scheduler_pool[i].io_worker = malloc(sizeof(pthread_t));
        scheduler_pool[i].queue = queue;
        thread_pool[i] = pthread_create(&(thread_pool[i]), NULL, (void *)run_scheduler, (void *)(&scheduler_pool[i]));
    }

    for (uint64_t j = 0; j < count; ++j) {
        pthread_join(thread_pool[j], NULL);
    }

    free(scheduler_pool);
    free(thread_pool);
    free(scheduler_states);

    pthread_mutex_destroy(scheduler_states_lock);
}

void run_scheduler(scheduler_t *sched) {
    queue_head *curr;
    task_t *curr_task;

    save_scheduler_context(sched->context);

    do {
        curr = queue_get(sched->queue);

        if (curr != NULL) {
            curr_task = (task_t *)(curr->item);
            set_active_workers(sched->id, WORKING);
            if ((curr_task->_state == WAITING) && check_subtasks_done(curr_task)) {
                sched->curr_task = curr_task;
                curr_task->scheduler_id = sched->id;
                swtch_context(&sched->context, curr_task->_context);
            } else if (curr_task->_state == NEW) {
                sched->curr_task = curr_task;
                sched->curr_task->_state = ACTIVE;
                curr_task->f(curr_task->args);
                sched->curr_task->done = 1;
                sched->curr_task->_state = DONE;
            } else {
                queue_add(curr, sched->queue);
            }
        } else if (get_active_workers() == 0) {
            break;
        } else {
            set_active_workers(sched->id, SLACKING);
        }
    } while (1);

    pthread_exit(NULL);
}

uint64_t get_active_workers() {
    pthread_mutex_lock(scheduler_states_lock);

    uint64_t acc = 0;

    for (uint64_t i = 0; i < g_count; ++i) {
        acc += scheduler_states[i];
    }

    pthread_mutex_unlock(scheduler_states_lock);

    return acc;
}

void set_active_workers(uint64_t id, scheduler_state_e state) {
    pthread_mutex_lock(scheduler_states_lock);

    scheduler_states[id] = state;

    pthread_mutex_unlock(scheduler_states_lock);
}
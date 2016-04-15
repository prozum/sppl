#ifndef SPPL_QUEUE_H
#define SPPL_QUEUE_H

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef _cas
# define _cas(ptr, oldval, newval) \
         __sync_bool_compare_and_swap(ptr, oldval, newval)
#endif

typedef struct queue_head {
    struct queue_head *next;
    void *item;
} queue_head;

typedef struct queue_root {
    struct queue_head *in_queue;
    struct queue_head *out_queue;

    pthread_mutex_t lock;
} queue_root;

queue_root *create_queue();
void queue_add(queue_head *new, queue_root *root);
queue_head *queue_get(queue_root *root);

#endif //SPPL_QUEUE_H

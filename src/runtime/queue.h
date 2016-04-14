#ifndef SPPL_QUEUE_H
#define SPPL_QUEUE_H

#include <pthread.h>
#include <stdlib.h>

typedef struct queue_head {
    struct queue_head *next;
    void *item;
} queue_head;

typedef struct queue_root {
    struct queue_head *in_queue;
    struct queue_head *out_queue;

    pthread_mutex_t lock;
} queue_root;

queue_root *alloc_queue_root();
void queue_put(queue_head *new, queue_root *root);
queue_head *queue_get(queue_root *root);

#endif //SPPL_QUEUE_H

#include "queue.h"

queue_root *create_queue()
{
    queue_root *root = malloc(sizeof(queue_root));

    pthread_mutex_init(&root->lock, NULL);

    root->in_queue = NULL;
    root->out_queue = NULL;
    return root;
}

void queue_add(queue_head *new, queue_root *root)
{
    do {
        new->next = root->in_queue;
    } while(!_cas(&root->in_queue, new->next, new));
}

queue_head *queue_get(queue_root *root)
{
    pthread_mutex_lock(&root->lock);

    if (!root->out_queue) {
        while (1) {
            queue_head *head = root->in_queue;

            if (!head) {
                break;
            }

            if (_cas(&root->in_queue, head, NULL)) {
                // Reverse the order
                while (head) {
                    queue_head *next = head->next;
                    head->next = root->out_queue;
                    root->out_queue = head;
                    head = next;
                }
                break;
            }
        }
    }

    queue_head *head = root->out_queue;

    if (head) {
        root->out_queue = head->next;
    }

    pthread_mutex_unlock(&root->lock);
    return head;
}
#include "util.h"

list_t *_str_to_list(char *str) {
    list_t *snode = 0;
    list_t *node;
    list_t *next_node;

    if (*str) {
        snode = node = ALLOC(sizeof(list_t));

         do {
            if (*(str + 1)) {
                next_node = ALLOC(sizeof(list_t));
            } else {
                next_node = 0;
            }

            node->data.char_v = *str++;
            node->next = next_node;
            node = next_node;
        } while (*str);
    }

    return  snode;
}

list_t *_make_args(int argc, char **argv) {
    list_t *node;
    list_t *last_node = 0;

    for (int i = argc - 1; i >= 0; --i) {
        node = ALLOC(sizeof(list_t));
        node->data.list_v = _str_to_list(argv[i]);
        node->next = last_node;
        last_node = node;
    }

    return last_node;
}

list_t *_concat_list(list_t *left, list_t *right) {
    list_t *snode;
    list_t *node = left;
    list_t *cpy_node;
    list_t *next_node;

    if (!node)
        return right;

    snode = cpy_node = ALLOC(sizeof(list_t));

    do {
        if (node->next) {
            next_node = ALLOC(sizeof(list_t));
        } else {
            next_node = right;
        }

        memcpy(&cpy_node->data, &node->data, sizeof(value_t));
        cpy_node->next = next_node;

        node = node->next;
        cpy_node = next_node;
    } while (node);

    return snode;
}
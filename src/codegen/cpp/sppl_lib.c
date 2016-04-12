//
// Created by hejsil on 4/8/16.
//

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "sppl_lib.h"

#define TYPE int

typedef struct list_block_s {
    struct list_block_s* next;
    int empty;
    TYPE value;
} list_block_t;

list_block_t* at(list_block_t* current, int i) {
    if (current->empty) {
        printf("Out of bound!");
        exit(1);
    }

    while (i-- > 0) {
        if (current->next->empty) {
            printf("Out of bound!");
            exit(1);
        }

        current = current->next;
    }

    return current;
}

TYPE value_at(list_block_t* current, int i) {
    return at(current, i)->value;
}

list_block_t* add(list_block_t* current, TYPE data) {
    list_block_t* res = malloc(sizeof(list_block_t));
    res->value = data;
    res->next = current;
    res->empty = 0;
    return res;
}

list_block_t* create_list(int count, ...) {
    int i;
    va_list args;
    list_block_t* res = malloc(sizeof(list_block_t));
    res->empty = 1;

    va_start(args, count);

    for (i = 0; i < count; i++) {
        res = add(res, va_arg(args, TYPE));
    }

    va_end(args);

    return res;
}

void main() {
    int i;
    list_block_t* list = create_list(0);

    for (i = 0; i < 1000; ++i) {
        list = add(list, i);
    }

    for (i = 0; i < 1001; ++i) {
        printf("%d\n", value_at(list, i));
    }
}
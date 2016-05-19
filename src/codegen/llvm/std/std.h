#ifndef STD_H
#define STD_H
#include "stdint.h"

//#define PUBLIC __attribute__((visibility("default")))
//#define PRIVATE __attribute__((visibility("hidden")))
#define SPPL_DECL __attribute__((annotate("sppl_decl")))

typedef enum type_enum {
    UNKNOWN,
    INT,
    FLOAT,
    CHAR,
    BOOL,
    STRING,
    LIST,
    TUPLE,
    SIGNATURE,
    GENERIC,
    EMPTYLIST,
    CUSTOM,
    VOID,
    NOT_USE = UINT64_MAX
} type_id;

typedef struct type {
    type_id id;
    struct type *subtypes[];
} type_t;

struct list;

typedef union value {
    double float_v;
    int64_t int_v;
    int64_t char_v;
    int64_t bool_v;
    struct list *list_v;
    union value *tuple_v;
    union value *ptr_v;
} value_t;

typedef struct list {
    value_t data;
    struct list *next;
} list_t;

// Print
SPPL_DECL void _print(value_t *val, type_t *type);
void _print_tuple(value_t *val, type_t *type);
void _print_list(list_t *val, type_t *type);
void _print_string(list_t *val);
void _print_type(type_t *type);
void _print_signature(type_t *type);

#endif

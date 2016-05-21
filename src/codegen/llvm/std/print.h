#ifndef SPPL_PRINT_H
#define SPPL_PRINT_H

#include "std.h"

void _print(value_t *val, type_t *type, int top);
void _print_tuple(value_t *val, type_t *type);
void _print_list(list_t *val, type_t *type);
void _print_string(list_t *val, int top);
void _print_type(type_t *type);
void _print_signature(type_t *type);

#endif //SPPL_PRINT_H

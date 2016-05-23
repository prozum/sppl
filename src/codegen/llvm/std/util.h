#ifndef SPPL_UTIL_H
#define SPPL_UTIL_H

#include "std.h"

list_t *_str_to_list(char *str);
list_t *_make_args(int argc, char** argv);
list_t *_concat_list(list_t *left, list_t *right);


#endif //SPPL_UTIL_H

#ifndef SPPL_IO_H
#define SPPL_IO_H

#include <stdio.h>
#include <setjmp.h>

#include "regs.h"
#include "task.h"

void sppl_print(char *str, task_t *task);

char *sppl_input();

#endif //SPPL_IO_H

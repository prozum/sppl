#define USE_UCONTEXT 1

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>
#if USE_UCONTEXT
#include <ucontext.h>
#endif
#include <sys/utsname.h>
#include <inttypes.h>
#include <stdarg.h>

#include "queue.h"

#define nil ((void*)0)

#define ulong task_ulong
#define uint task_uint
#define uchar task_uchar
#define ushort task_ushort
#define uvlong task_uvlong
#define vlong task_vlong

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long long uvlong;
typedef long long vlong;

void print(char *str);

enum
{
    STACK = 8192
};

typedef struct context_s
{
    ucontext_t	uc;
} context_t;
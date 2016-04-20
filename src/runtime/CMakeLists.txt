#add_library(runtime queue.h queue.c runtime.h runtime.c  context.c 386-ucontext.h amd64-ucontext.h mips-ucontext.h power-ucontext.h channel.c task.c task.h taskimpl.h asm.S print.c)

add_executable(runtime main.c queue.h queue.c context.c 386-ucontext.h amd64-ucontext.h mips-ucontext.h task.c task.h asm.S print.c defs.h runtime.c runtime.h)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#add_executable(runtime main.c queue.h queue.c context.c 386-ucontext.h amd64-ucontext.h mips-ucontext.h task.c task.h asm.S print.c defs.h runtime.c)

add_library(runtime queue.h queue.c runtime.h runtime.c  context.c 386-ucontext.h amd64-ucontext.h mips-ucontext.h power-ucontext.h task.c task.h asm.S print.c)
target_link_libraries(runtime Threads::Threads)


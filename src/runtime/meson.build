thread_dep = dependency('threads')

runtime_lib = library('runtime', 
                      'queue.c', 'runtime.c', 'context.c', 'task.c', 'asm.S', 'print.c',
                      dependencies: thread_dep)

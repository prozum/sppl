subdir('common')
subdir('parser')

if optimizer_enabled or ccpp_enabled
  subdir('optimizer')
endif

subdir('semantics')
subdir('codegen')
subdir('compiler')

if ccpp_enabled
  subdir('runtime')
endif
if jit_enabled or interpreter_enabled
  subdir('jit')
endif

scc_exe = executable('scc', 'Main.cpp', 
                     dependencies: compiler_dep,
                     include_directories: incdir)

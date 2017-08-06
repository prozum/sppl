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

libs = [common_lib, parser_lib, semantics_lib, compiler_lib]
libs += optimizer_enabled ? optimizer_lib : []

deps = cllvm_enabled ? llvm_dep : []

scc_exe = executable('scc', 'Main.cpp', 
                     link_with: libs,
                     dependencies: deps,
                     include_directories: incdir)

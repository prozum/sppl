libs = [common_lib, parser_lib, semantics_lib, cllvm_lib]

if optimizer_enabled
  libs += optimizer_lib
endif
if runtime_enabled
  libs += runtime_lib
endif

jit_lib = library('jit', 
                  'SpplJit.cpp',
                  link_with: libs,
                  dependencies: llvm_dep,
                  include_directories: incdir)

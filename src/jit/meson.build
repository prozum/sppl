deps = [cllvm_dep, parser_dep]

if optimizer_enabled
  deps += optimizer_dep
endif
if runtime_enabled
  deps += runtime_dep
endif

jit_lib = library('jit', 
                  'SpplJit.cpp',
                  dependencies: deps)

jit_dep = declare_dependency(
    link_with: jit_lib,
    dependencies: deps)

deps = [parser_dep, semantics_dep, pprinter_dep]

if ccpp_enabled
  deps += [cpar_dep, cc_dep]
endif
if chaskell_enabled
  deps += chaskell_dep
endif
if cgnuasm_enabled
  deps += cgnuasm_dep
endif
if cllvm_enabled
  deps += cllvm_dep
endif
if optimizer_enabled
  deps += optimizer_dep
endif

compiler_lib = library('compiler', 
                       'Compiler.cpp',
                       dependencies: deps,
                       include_directories: incdir)

compiler_dep = declare_dependency(
    link_with: compiler_lib,
    dependencies: deps)

libs = [common_lib, parser_lib, semantics_lib, pprinter_lib]
deps = []

libs += ccpp_enabled ? [cpar_lib, cc_lib] : []
libs += chaskell_enabled ? chaskell_lib : []
libs += cgnuasm_enabled ? cgnuasm_lib : []
libs += cllvm_enabled ? cllvm_lib : []
libs += optimizer_enabled ? optimizer_lib : []

deps += cllvm_enabled ? llvm_dep : []

compiler_lib = library('compiler', 
                       'Compiler.cpp',
                       link_with: libs,
                       dependencies: deps,
                       include_directories: incdir)

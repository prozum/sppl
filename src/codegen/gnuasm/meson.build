cgnuasm_lib = library('cgnuasm', 
                      'GasCodeGen.cpp', 'GasCodeGen.h',
                      dependencies: [parser_dep, semantics_dep],
                      include_directories: incdir)

cgnuasm_dep = declare_dependency(
    link_with: cgnuasm_lib)

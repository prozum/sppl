chaskell_lib = library('chaskell', 
                       'HCodeGen.cpp', 'HCodeGen.h',
                       dependencies: [parser_dep, semantics_dep],
                       include_directories: incdir)

chaskell_dep = declare_dependency(
    link_with: chaskell_lib)

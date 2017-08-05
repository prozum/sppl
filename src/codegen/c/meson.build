cc_lib = library('cc', 
                 'CCodeGen.cpp', 'CTree.cpp',
                 dependencies: [parser_dep, semantics_dep],
                 include_directories: incdir)

cc_dep = declare_dependency(
    link_with: cc_lib)

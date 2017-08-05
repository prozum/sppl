cpar_lib = library('cpar', 
                   'CParCodeGen.cpp',
                   dependencies: [common_dep, parser_dep, semantics_dep, cc_dep],
                   include_directories: incdir)

cpar_dep = declare_dependency(
    link_with: cpar_lib)

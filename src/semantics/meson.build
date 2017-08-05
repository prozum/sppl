semantics_lib = library('semantics', 
                        'TypeChecker.cpp', 'ScopeGenerator.cpp',
                        dependencies: [common_dep, parser_dep],
                        include_directories: incdir)

semantics_dep = declare_dependency(
    link_with: semantics_lib)

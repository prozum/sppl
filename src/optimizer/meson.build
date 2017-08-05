optimizer_lib = library('optimizer', 
                        'GeneralOptimizer.cpp',
                        link_with: [common_lib, parser_lib],
                        include_directories: incdir)

optimizer_dep = declare_dependency(
    link_with: optimizer_lib)

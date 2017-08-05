pprinter_lib = library('pprinter', 
                       'Printer.cpp',
                       dependencies: [common_dep, parser_dep],
                       include_directories: incdir)

pprinter_dep = declare_dependency(
    link_with: pprinter_lib)

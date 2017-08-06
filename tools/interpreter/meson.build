libs = [common_lib, parser_lib, semantics_lib, cllvm_lib, jit_lib]

libs += optimizer_enabled ? optimizer_lib : []

sppl_exe = executable('sppl', 
                      'Main.cpp',
                      dependencies: llvm_dep,
                      link_with: libs,
                      include_directories: incdir)

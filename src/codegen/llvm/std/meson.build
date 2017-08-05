clang = find_program('clang')
xxd = find_program('xxd')

stdlib_bc = custom_target('stdlib_bc',
                          input: ['std.c'],
                          output: 'std.bc',
                          command: [clang, '-emit-llvm', '-c', '@INPUT@', '-o', '@OUTPUT@'])

stdlib_bin = custom_target('stdlib_bin',
                           input: stdlib_bc,
                           output: 'std-bin.h',
                           capture: true,
                           command: [xxd, '-i', '@INPUT@'])

stdlib_lib = library('stdlib', 'std.c', 'std.h')

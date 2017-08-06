subdir('std')

llvm_dep = dependency('LLVM', 
                      modules: ['support', 'core', 'irreader', 'mcjit', 'native', 'bitwriter'])

cllvm_lib = library('cllvm', 
                    'LLVMCodeGen.cpp', 'Pattern.cpp', 'Expression.cpp', 
                    'BinaryOp.cpp', 'Type.cpp', 'Function.cpp', 'UnaryOp.cpp', 
                    'Utils.cpp', 'Prefix.cpp', 'Std.cpp', stdlib_bin,
                    dependencies: [llvm_dep],
                    link_with: [common_lib, parser_lib, semantics_lib],
                    include_directories: incdir)

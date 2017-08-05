project('sppl', ['cpp','c'])

debug_enabled = get_option('debug')
static_enabled = get_option('static')
line_enabled = get_option('line')
ccpp_enabled = get_option('ccpp')
chaskell_enabled = get_option('chaskell')
cgnuasm_enabled = get_option('cgnuasm')
cllvm_enabled = get_option('cllvm')
optimizer_enabled = get_option('optimizer')
runtime_enabled = get_option('runtime')
interpreter_enabled = get_option('interpreter')
jit_enabled = get_option('jit')
test_enabled = get_option('test')

# Set C/C++ defines from CMake options
if ccpp_enabled
    add_global_arguments('-DCCPP', language : 'cpp')
endif
if chaskell_enabled
    add_global_arguments('-DCHASKELL', language : 'cpp')
endif
if cgnuasm_enabled
    add_global_arguments('-DCGNUASM', language : 'cpp')
endif
if cllvm_enabled
    add_global_arguments('-DCLLVM', language : 'cpp')
endif
if optimizer_enabled
    add_global_arguments('-DOPTIMIZER', language : 'cpp')
endif
if test_enabled
    add_global_arguments('-DTEST', language : 'cpp')
endif

incdir = include_directories(['src/common', 'src/semantics', 'src/parser', 
                              'src/codegen/c', 'src/codegen/cpar', 'src/codegen/haskell', 
                              'src/codegen/gnuasm', 'src/codegen/llvm', 'src/codegen/llvm/std', 
                              'src/codegen/pprinter', 'src/compiler', 'src/optimizer', 'src/jit',
                              'tools/interpreter'])

subdir('src')
subdir('tools')

if test_enabled
  subdir('test')
endif

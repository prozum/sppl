add_library(jit SpplJit.cpp SpplJit.h)
target_link_libraries(jit optimizer semantics cllvm)

add_library(jit SpplJit.cpp SpplJit.h)

if (OPTIMIZER)
    set (LIBS ${LIBS} optimizer)
endif()

target_link_libraries(jit semantics cllvm ${LIBS})

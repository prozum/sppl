add_library(compiler Compiler.cpp Compiler.h)

set (BACKEND_LIBS pprinter)
if (CCPP)
    set (BACKEND_LIBS ${BACKEND_LIBS} ccpp)
endif()
if (CHASKELL)
    set (BACKEND_LIBS ${BACKEND_LIBS} chaskell)
endif()
if (CGNUASM)
    set (BACKEND_LIBS ${BACKEND_LIBS} cgnuasm)
endif()
if (CLLVM)
    set (BACKEND_LIBS ${BACKEND_LIBS} cllvm)
endif()

target_link_libraries(compiler common parser ${BACKEND_LIBS})


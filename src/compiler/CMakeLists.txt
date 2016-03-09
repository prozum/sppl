add_library(compiler Compiler.cpp Compiler.h)
target_link_libraries(compiler parser common codegen)


add_library(cpar CParCodeGen.cpp CParCodeGen.h)
target_link_libraries(cpar common parser semantics)

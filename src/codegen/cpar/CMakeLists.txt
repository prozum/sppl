add_library(cpar CParCodeGenerator.cpp CParCodeGenerator.h)
target_link_libraries(cpar common parser semantics)

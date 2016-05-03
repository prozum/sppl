add_library(ccpp CCodeGen.cpp CCodeGen.h CExpressions.cpp CPatterns.cpp)
target_link_libraries(ccpp common parser semantics)

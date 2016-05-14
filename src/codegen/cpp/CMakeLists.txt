add_library(ccpp CCodeGenOld.cpp CCodeGenOld.h CExpressions.cpp CPatterns.cpp)
target_link_libraries(ccpp common parser semantics)

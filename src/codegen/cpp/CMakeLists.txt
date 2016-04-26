add_library(ccpp CppCodeGenerator.cpp CppCodeGenerator.h CExpressions.cpp CPatterns.cpp)
target_link_libraries(ccpp common parser semantics)

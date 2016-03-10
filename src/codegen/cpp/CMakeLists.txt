add_library(ccpp CppCodeGenerator.cpp CppCodeGenerator.h)
target_link_libraries(ccpp common parser)

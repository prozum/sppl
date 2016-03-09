add_library(cppc CppCodeGenerator.cpp CppCodeGenerator.h)
target_link_libraries(cppc common parser compiler visitor)

add_library(cgnuasm GasCodeGenerator.cpp GasCodeGenerator.h)
target_link_libraries(cgnuasm common parser compiler visitor)

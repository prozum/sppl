add_library(cgnuasm GasCodeGen.cpp GasCodeGen.h)
target_link_libraries(cgnuasm common parser)

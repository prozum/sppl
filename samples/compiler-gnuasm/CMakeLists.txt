add_executable(cgnuasm Main.cpp GasCodeGenerator.cpp GasCodeGenerator.h)
target_link_libraries(cgnuasm common parser compiler visitor)
add_executable(cgnuasm Main.cpp GasCodeGenerator.cpp)
target_link_libraries(cgnuasm common parser compiler)

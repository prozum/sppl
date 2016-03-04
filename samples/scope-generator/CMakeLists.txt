add_executable(cscope main.cpp ScopeGenerator.cpp)
target_link_libraries(cscope common parser compiler)

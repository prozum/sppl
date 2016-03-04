add_executable(cscope main.cpp ScopeGenerator.cpp TypeChecker.cpp TypeChecker.h)
target_link_libraries(cscope common parser compiler)

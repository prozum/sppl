add_library(typecheck TypeChecker.cpp ScopeGenerator.cpp TypeChecker.h ScopeGenerator.h)
target_link_libraries(typecheck common)

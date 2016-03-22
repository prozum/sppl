add_library(semantics TypeChecker.cpp ScopeGenerator.cpp TypeChecker.h ScopeGenerator.h CaseChecker.cpp CaseChecker.h)
target_link_libraries(semantics common)

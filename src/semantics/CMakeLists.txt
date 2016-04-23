add_library(semantics TypeChecker.cpp ScopeGenerator.cpp TypeChecker.h ScopeGenerator.h GenericUnfolder.cpp GenericUnfolder.h)
target_link_libraries(semantics common)

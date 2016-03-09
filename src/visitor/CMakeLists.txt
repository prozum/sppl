add_library(visitor CodeGenerator.cpp CodeGenerator.h Optimizer.cpp Optimizer.h TypeChecker.cpp TypeChecker.h)
target_link_libraries(visitor common)

add_library(visitor Visitor.cpp Visitor.h CodeGenerator.cpp CodeGenerator.h Optimizer.cpp Optimizer.h TypeChecker.cpp TypeChecker.h)
target_link_libraries(visitor common)

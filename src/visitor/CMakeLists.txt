add_library(visitor CodeGenerator.cpp CodeGenerator.h Optimizer.cpp Optimizer.h HPInkJetVisitor.cpp HPInkJetVisitor.h TypeChecker.cpp TypeChecker.h SkeletonVisitor.cpp SkeletonVisitor.h)
target_link_libraries(visitor common)

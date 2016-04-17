add_library(optimizer GeneralOptimizer.cpp ParallelOptimizer.cpp GeneralOptimizer.h ParallelOptimizer.h)
target_link_libraries(optimizer parser common)

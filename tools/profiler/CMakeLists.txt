add_library(profiler Main.cpp Profiler.cpp Profiler.h)
target_link_libraries(profiler interpreter common)

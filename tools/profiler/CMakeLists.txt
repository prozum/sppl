add_library(profiler Main.cpp Profiler.cpp Profiler.h DataGenerator.cpp DataGenerator.h)
target_link_libraries(profiler interpreter parser common)

add_library(profiler Main.cpp Profiler.cpp Profiler.h Interpreter.cpp Interpreter.h)
target_link_libraries(profiler common)

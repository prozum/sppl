if (PROFILER)
    add_subdirectory(profiler)
    include_directories(profiler)
endif()

if (INTERPRETER)
    add_subdirectory(interpreter)
    include_directories(interpreter)
endif()

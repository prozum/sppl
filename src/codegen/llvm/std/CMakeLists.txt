add_custom_command(OUTPUT std-bin.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND clang -emit-llvm -c std.c -o std.bc
        COMMAND xxd -i std.bc > std-bin.h)
add_custom_target(std DEPENDS std-bin.h)

add_library(stdlib std.c std.h)

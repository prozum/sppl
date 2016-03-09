add_subdirectory(common)
add_subdirectory(parser)
add_subdirectory(visitor)
add_subdirectory(codegen)
add_subdirectory(compiler)

add_executable(scc Main.cpp)
target_link_libraries(scc compiler)

install(TARGETS scc
        RUNTIME DESTINATION bin)

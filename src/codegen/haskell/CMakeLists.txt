add_executable(hc Main.cpp HCodeGenerator.cpp HCodeGenerator.h HCompiler.cpp HCompiler.h)
target_link_libraries(hc common parser compiler visitor)

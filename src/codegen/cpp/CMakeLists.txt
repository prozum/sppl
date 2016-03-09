add_executable(cppc Main.cpp CppCodeGenerator.cpp CppCodeGenerator.h CppCompiler.cpp CppCompiler.h)
target_link_libraries(cppc common parser compiler visitor)

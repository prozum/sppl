add_executable(cc Main.cpp CCodeGenerator.cpp CCodeGenerator.h CCompiler.cpp CCompiler.h)
target_link_libraries(cc common parser compiler visitor)

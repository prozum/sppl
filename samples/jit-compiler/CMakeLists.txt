add_executable(jit Main.cpp SpplJit.cpp SpplJit.h)
target_link_libraries(jit common parser compiler semantics)

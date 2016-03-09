add_library(hc HCodeGenerator.cpp HCodeGenerator.h)
target_link_libraries(hc common parser compiler visitor)

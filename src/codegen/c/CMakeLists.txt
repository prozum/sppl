add_library(cc CCodeGen.cpp CCodeGen.h CTree.cpp CTree.h)
target_link_libraries(cc common parser semantics)

add_library(chaskell HCodeGenerator.cpp HCodeGenerator.h)
target_link_libraries(chaskell common parser compiler)

add_library(parser NodeBuilder.cpp NodeBuilder.h Parser.cpp Parser.h Scanner.cpp Scanner.h)
target_link_libraries(parser common)

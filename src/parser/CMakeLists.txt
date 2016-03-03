#add_library(parser NodeBuilder.cpp NodeBuilder.h Parser.cpp Parser.h Scanner.cpp Scanner.h)
find_package(BISON)
find_package(FLEX)

BISON_TARGET(SpplParser
        Parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp
        COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/Parser.hpp --skeleton=/usr/share/bison/lalr1.cc")
FLEX_TARGET(SpplScanner
        Scanner.l
        ${CMAKE_CURRENT_BINARY_DIR}/Scanner.cpp)

ADD_FLEX_BISON_DEPENDENCY(SpplScanner SpplParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(parser Parser.cpp Scanner.cpp NodeBuilder.cpp NodeBuilder.h)
target_link_libraries(parser common)


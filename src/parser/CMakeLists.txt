#add_library(parser NodeBuilder.cpp NodeBuilder.h Parser.cpp Parser.h Scanner.cpp Scanner.h)
find_package(BISON)
find_package(FLEX)

BISON_TARGET(SpplParser Parser.y ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp COMPILE_FLAGS "--defines=Parser.h")
FLEX_TARGET(SpplScanner Scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(SpplScanner SpplParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(parser
        ${BISON_SpplParser_OUTPUTS}
        ${FLEX_SpplScanner_OUTPUTS}
        Scanner.h Driver.h Driver.cpp)
target_link_libraries(parser common)


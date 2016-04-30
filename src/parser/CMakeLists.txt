find_package(BISON)
find_package(FLEX)

if (LINE)
    flex_target(SpplScanner flex/scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp)
    bison_target(SpplParser bison/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp COMPILE_FLAGS "--defines=Parser.h -S bison/skeleton.m4")
ELSE()
    flex_target(SpplScanner flex/scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp COMPILE_FLAGS "--noline")
    bison_target(SpplParser bison/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp COMPILE_FLAGS "--defines=Parser.h -S bison/skeleton.m4 --no-lines")
ENDIF()

add_flex_bison_dependency(SpplScanner SpplParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(parser
        ${BISON_SpplParser_OUTPUTS}
        ${FLEX_SpplScanner_OUTPUTS}
        Scanner.h Driver.h Driver.cpp
        CodeGenerator.h CodeGenerator.cpp)

target_link_libraries(parser common)


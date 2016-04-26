find_package(BISON)
find_package(FLEX)

add_custom_target(FlexSkeleton
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/flex/
        COMMAND sed 's/m4_/m4postproc_/g\;s/m4preproc_/m4_/g' skeleton.m4 | m4 -P -DFLEX_MAJOR_VERSION=2 -DFLEX_MINOR_VERSION=5 -DFLEX_SUBMINOR_VERSION=39 | sed 's/m4postproc_/m4_/g' >flex.skl)

#bison_target(SpplParser Parser.y ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp COMPILE_FLAGS "--defines=Parser.h --no-line")
bison_target(SpplParser bison/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp COMPILE_FLAGS "--defines=Parser.h -S bison/skeleton.m4")
flex_target(SpplScanner flex/scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp COMPILE_FLAGS "-S flex/flex.skl")
add_flex_bison_dependency(SpplScanner SpplParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(parser
        ${BISON_SpplParser_OUTPUTS}
        ${FLEX_SpplScanner_OUTPUTS}
        Scanner.h Driver.h Driver.cpp
        CodeGenerator.h CodeGenerator.cpp)
add_dependencies(parser FlexSkeleton)

target_link_libraries(parser common)


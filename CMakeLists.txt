cmake_minimum_required(VERSION 2.8)
project(sppl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti")


option(DEBUG "Debug mode" ON)

if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-unused-parameter")
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

option(CCPP "C++ Compiler" ON)
option(CHASKELL "Haskell Compiler" ON)
option(CGNUASM "GNU Assembly Compiler" ON)
option(CLLVM "LLVM Intermediate Representation Compiler" ON)
#option(SCOPE "Scope Generator Sample" OFF)

add_definitions(-DCCPP)
add_definitions(-DCHASKELL)
add_definitions(-DCGNUASM)

include_directories(
        src/common
        src/parser
        src/codegen/cpp
        src/codegen/haskell
        src/codegen/gnuasm
        src/codegen/llvm
        src/codegen/pprinter
        src/compiler
)

add_subdirectory(src)
#add_subdirectory(samples)
add_subdirectory(tools)
add_subdirectory(tests)

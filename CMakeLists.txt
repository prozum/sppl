cmake_minimum_required(VERSION 2.8)
project(sppl)

option(DEBUG "Debug mode" ON)
option(CCPP "C++ Compiler" OFF)
option(CHASKELL "Haskell Compiler" OFF)
option(CGNUASM "GNU Assembly Compiler" OFF)
option(CLLVM "LLVM Intermediate Representation Compiler" OFF)
option(PROFILER "Profiler description" OFF)
option(INTERPRETER "Interpreter description" OFF)
option(JIT "LLVM JIT Compiler" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-unused-parameter")
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CCPP)
    add_definitions(-DCCPP)
endif()
if (CHASKELL)
    add_definitions(-DCHASKELL)
endif()
if (CGNUASM)
    add_definitions(-DCGNUASM)
endif()
if (CLLVM)
    add_definitions(-DCLLVM)
endif()

include_directories(
        src/common
        src/semantics
        src/parser
        src/codegen/cpp
        src/codegen/haskell
        src/codegen/gnuasm
        src/codegen/llvm
        src/codegen/pprinter
        src/compiler
)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(tools)
add_subdirectory(tests)

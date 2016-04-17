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
option(TESTS "Tests" OFF)

# Flags that enable C++14 and use of static libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static")
# Flags that allow compilation with clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -fPIE")

if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pedantic -Wno-unused-parameter")
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
        src/optimizer
        src/jit
)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(tools)
if (TESTS)
    add_subdirectory(tests)
endif()

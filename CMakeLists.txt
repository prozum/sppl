cmake_minimum_required(VERSION 2.8)
project(sppl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti")

if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-unused-parameter")
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(
        src/common
        src/parser
        src/visitor
        src/compiler
)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(tools)
add_subdirectory(tests)

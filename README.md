[![Travis: Build Status](https://travis-ci.org/prozum/sppl.svg?branch=master)](https://travis-ci.org/prozum/sppl)
[![Coverity: Analysis Status](https://scan.coverity.com/projects/8011/badge.svg)](https://scan.coverity.com/projects/8011)

# SPPL
Simple Parallel Programming Language

Dependencies
--------------------
- CMake		≥ 3.1
- flex	≥ 2.5
- bison	≥ 3.0

Compile Instructions
--------------------
SPPL uses CMake as the build system.

Here is a description of SPPL's CMake flags:

CMake Flags     | Default Value | Meaning
-------------   | ------------- | -------------
-DDEBUG         |       1       | Build in Debug Mode
-DCCPP          |       1       | Build C Code Generators
-DCLLVM         |       0       | Build LLVM Code Generator
-DCHASKELL      |       0       | Build Haskell Code Generator
-DCGNUASM       |       0       | Build GNU Assembly Code Generator
-DINTERPRETER   |       0       | Build LLVM-based Interpreter


C++ Style
--------------------
http://llvm.org/docs/CodingStandards.html

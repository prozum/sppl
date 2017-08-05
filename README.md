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
SPPL uses Meson as build system.

Description of SPPL's Meson flags:

Meson Flags     | Default Value | Meaning
-------------   | ------------- | -------------
-Dccpp          |     true      | Build C Code Generators
-Dcllvm         |     false     | Build LLVM Code Generator
-Dchaskell      |     false     | Build Haskell Code Generator
-Dcgnuasm       |     false     | Build GNU Assembly Code Generator
-Dinterpreter   |     false     | Build LLVM-based Interpreter
-Dtest          |     false     | Build unit tests


C++ Style
--------------------
http://llvm.org/docs/CodingStandards.html

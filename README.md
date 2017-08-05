[![Travis: Build Status](https://travis-ci.org/prozum/sppl.svg?branch=master)](https://travis-ci.org/prozum/sppl)
[![Coverity: Analysis Status](https://scan.coverity.com/projects/8011/badge.svg)](https://scan.coverity.com/projects/8011)

# SPPL
Simple Parallel Programming Language

Dependencies
--------------------
- Meson		≥ 0.41.0
- flex	≥ 2.5
- bison	≥ 3.0

Compile Instructions
--------------------
SPPL uses Meson as the build system.

Description of SPPL's Meson options:

Meson Flags     | Default Value | Meaning
----------------| ------------- | -------------
ccpp            |     true      | Build C Code Generators
cllvm           |     false     | Build LLVM Code Generator
chaskell        |     false     | Build Haskell Code Generator
cgnuasm         |     false     | Build GNU Assembly Code Generator
interpreter     |     false     | Build LLVM-based Interpreter
test            |     false     | Build unit tests


C++ Style
--------------------
http://llvm.org/docs/CodingStandards.html

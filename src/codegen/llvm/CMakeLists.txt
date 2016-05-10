find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(cllvm LLVMCodeGen.cpp LLVMCodeGen.h Pattern.cpp Expression.cpp BinaryOp.cpp Type.cpp Function.cpp UnaryOp.cpp Utils.cpp)
llvm_map_components_to_libnames(LLVM_LIBS support core irreader mcjit native bitwriter)

target_link_libraries(cllvm common parser ${LLVM_LIBS})

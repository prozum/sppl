find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(cllvm LLVMCodeGenerator.cpp LLVMCodeGenerator.h)
if (JIT)
    llvm_map_components_to_libnames(LLVM_LIBS support core irreader mcjit native)
else()
    llvm_map_components_to_libnames(LLVM_LIBS support core)
endif()

target_link_libraries(cllvm common parser ${LLVM_LIBS})

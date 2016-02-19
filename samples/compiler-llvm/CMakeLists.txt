find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(cllvm Main.cpp)
llvm_map_components_to_libnames(llvm_libs support core irreader mcjit native)
target_link_libraries(cllvm common parser compiler ${llvm_libs})

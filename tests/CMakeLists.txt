find_package(PkgConfig)
pkg_search_module(CPPUNIT cppunit)

add_executable(tests Test.cpp AddTest.cpp BinTest.cpp CasTest.cpp DivTest.cpp FuncTest.cpp KeywordTest.cpp ListTest.cpp ModTest.cpp MulTest.cpp SigTest.cpp SubTest.cpp TupleTest.cpp Test.h ScopeTest.cpp DotTest.cpp PrecedenceTest.cpp WildTest.cpp FileTest.cpp WhenTest.cpp)
target_link_libraries(tests compiler ${CPPUNIT_LIBRARIES})
target_include_directories(tests PUBLIC ${CPPUNIT_INCLUDE_DIRS})
target_compile_options(tests PUBLIC ${CPPUNIT_CFLAGS_OTHER})

add_custom_command(TARGET tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/tests/source/unittest $<TARGET_FILE_DIR:tests>)
add_custom_command(TARGET tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/runtime $<TARGET_FILE_DIR:tests>)
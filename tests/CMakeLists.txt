find_package(PkgConfig)
pkg_search_module(CPPUNIT cppunit)


add_executable(tests Main.cpp)
target_link_libraries(tests ${CPPUNIT_LIBRARIES})
target_include_directories(tests PUBLIC ${CPPUNIT_INCLUDE_DIRS})
target_compile_options(tests PUBLIC ${CPPUNIT_CFLAGS_OTHER})
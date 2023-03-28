cmake_minimum_required(VERSION 3.15)
project(PerfLib CXX C)

include(GNUInstallDirs)

add_library(
    perflib STATIC
    include/perflib/perflib.h
    src/perf-counters.h
    src/perf-counters.c
)

target_include_directories(
    perflib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

add_executable(
    perflib-test

    tests/perflib-test.cpp
)

target_link_libraries(
    perflib-test PRIVATE perflib
)

set_target_properties(perflib PROPERTIES PUBLIC_HEADER include/perflib/perflib.h)


install(
	TARGETS perflib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/perflib
)



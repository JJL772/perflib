cmake_minimum_required(VERSION 3.15)
project(PerfLib CXX C)

add_library(
    perflib

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

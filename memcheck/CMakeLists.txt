# Project
cmake_minimum_required(VERSION 3.16)
project(memory_check)

set(COMMON_FLAGS "-rdynamic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

# inc and src directory
include_directories(inc/)
aux_source_directory(src/ SOURCE_FILE)

# Library
add_library(memory_check SHARED ${SOURCE_FILE})

# configuration script
file(COPY script/run DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# test
## normal
add_executable(test_c test/test.c)
add_executable(test_cpp test/test.cpp)
## mat
add_executable(mat_c test/mat.c)
add_executable(mat_cpp test/mat.cpp)
## pipe
add_executable(pipe test/pipe.c)
## class
add_executable(line test/line.cpp)
## Python
file(COPY test/test_py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# reference
add_executable(redirect_c ref/mem.c ref/c_test.c)
add_executable(redirect_cpp ref/op.cpp)
add_executable(backtrace ref/back.c)

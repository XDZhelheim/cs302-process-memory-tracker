# Project
cmake_minimum_required(VERSION 3.16)
project(memory_check)

set(COMMON_FLAGS "-rdynamic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

# Library
add_library(memory_check SHARED 
    src/main_redirect.c src/main_log.c src/trace.cpp 
    src/memory_redirect.c src/memory_log.cpp src/memory_node.cpp 
    src/file_descriptor_redirect.c src/file_descriptor_log.cpp src/file_descriptor_node.cpp 
    src/file_handler_redirect.c src/file_handler_log.cpp src/file_handler_node.cpp)

# configuration script
configure_file(script/run ${CMAKE_CURRENT_BINARY_DIR}/run COPYONLY)

# test
## normal
add_executable(test_c test/test.c)
add_executable(test_cpp test/test.cpp)
## mat
add_executable(mat_c test/mat.c)
add_executable(mat_cpp test/mat.cpp)
## class
add_executable(line test/line.cpp)

# reference
add_executable(redirect_c ref/mem.c ref/c_test.c)
add_executable(redirect_cpp ref/op.cpp)
add_executable(backtrace ref/back.c)

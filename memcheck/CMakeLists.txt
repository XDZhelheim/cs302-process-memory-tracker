# Project
cmake_minimum_required(VERSION 3.16)
project(memory_check)
set(COMMON_FLAGS "-funwind-tables -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual \
                  -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef \
                  -Wuninitialized -rdynamic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_FLAGS} -Wstrict-prototypes -Wmissing-prototypes")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

# C++ compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Library
add_library(memory_check SHARED 
    src/main_redirect.c src/main_log.c src/trace.cpp 
    src/memory_redirect.c src/memory_log.cpp src/memory_node.cpp 
    src/file_descriptor_redirect.c src/file_descriptor_log.cpp src/file_descriptor_node.cpp 
    src/file_handler_redirect.c src/file_handler_log.cpp src/file_handler_node.cpp)

target_compile_features(memory_check PRIVATE cxx_variadic_templates)
# target_link_libraries(redirect pthread)

# configuration
configure_file(script/run ${CMAKE_CURRENT_BINARY_DIR}/run COPYONLY)

# test
## normal
add_executable(test_c test/test.c)
add_executable(test_cpp test/test.cpp)
## mat
add_executable(mat_c test/mat.c)
add_executable(mat_cpp test/mat.cpp)

# reference
add_executable(redirect_c ref/mem.c ref/c_test.c)
add_executable(redirect_cpp ref/op.cpp)
add_executable(backtrace ref/back.c)

# Project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(abc VERSION 1.0 LANGUAGES C CXX)
set(COMMON_FLAGS "-funwind-tables -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual \
                  -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code -Wundef \
                  -Wuninitialized ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_FLAGS} -Wstrict-prototypes -Wmissing-prototypes")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

# C++ compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Library
add_library(abc SHARED src/abc.c)
target_compile_features(abc PRIVATE cxx_variadic_templates)
target_link_libraries(abc pthread dl)

add_executable(test src/test.c)
add_executable(tt src/tt.cpp)

# -rdynamic
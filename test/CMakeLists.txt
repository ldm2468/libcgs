cmake_minimum_required(VERSION 3.20)
project(cgs_test C)

set(CMAKE_C_STANDARD 90)

add_executable(cgs_test vector.c ../cgs_vector.h ../cgs_common.h ../cgs_reset.h)
target_include_directories(cgs_test PRIVATE ..)

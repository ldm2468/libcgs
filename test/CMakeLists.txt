cmake_minimum_required(VERSION 3.20)
project(cgs_test C)

set(CMAKE_C_STANDARD 99)

include_directories(PRIVATE ..)
add_executable(test_vector vector.c ../cgs_vector.h ../cgs_common.h cnit/cnit.h cnit/cnit_main.h)
add_executable(test_list list.c ../cgs_list.h ../cgs_common.h cnit/cnit.h cnit/cnit_main.h)

cmake_minimum_required(VERSION 3.20)
project(cgs_test C)
enable_testing()

set(CMAKE_C_STANDARD 99)

include_directories(PRIVATE ..)
add_executable(test_vector vector.c ../cgs_vector.h ../cgs_common.h cnit/cnit.h cnit/cnit_main.h)
add_executable(test_list list.c ../cgs_list.h ../cgs_common.h cnit/cnit.h cnit/cnit_main.h)
add_executable(test_map map.c ../cgs_map.h ../cgs_common.h cnit/cnit.h cnit/cnit_main.h)

add_test(NAME test_vector COMMAND test_vector)
add_test(NAME test_list COMMAND test_list)
add_test(NAME test_map COMMAND test_map)

cmake_minimum_required(VERSION 3.18.4)


project(LIBMYCRYPTO)

add_compile_options(-Wall -Wextra -pedantic -O3)


find_library(CRITERION_LIB criterion)
if(NOT CRITERION_LIB)
  message(FATAL_ERROR "criterion library not found")
endif()

add_subdirectory(src/libmycrypto)
add_subdirectory(tools)
add_subdirectory(test)

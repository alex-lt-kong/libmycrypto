cmake_minimum_required(VERSION 3.18.4)


project(LIBMYCRYPTO)

add_compile_options(-Wall -Wextra -pedantic -O3)


add_subdirectory(src/mycrypto)
add_subdirectory(tools)
add_subdirectory(test)

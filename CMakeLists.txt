cmake_minimum_required(VERSION 3.18.4)

option(BUILD_ASAN "Build with AddressSanitizer to detect memory error" OFF)
if(BUILD_ASAN)
    MESSAGE("-- AddressSanitizer WILL be compiled in as BUILD_ASAN=ON")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
else()
    MESSAGE("-- AddressSanitizer will NOT be compiled in as BUILD_ASAN=OFF")
endif(BUILD_ASAN)

project(LIBMYCRYPTO)

add_compile_options(-Wall -Wextra -pedantic -O1)



add_subdirectory(src/mycrypto)
add_subdirectory(tools)
add_subdirectory(test)

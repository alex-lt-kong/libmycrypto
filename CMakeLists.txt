cmake_minimum_required(VERSION 3.18.4)

project(LIBMYCRYPTO)

option(BUILD_ASAN "Build with AddressSanitizer to detect memory error" OFF)
option(BUILD_MSAN "Build with MemorySanitizer to detect memory error" OFF)
option(BUILD_UBSAN "Build with UndefinedBehaviorSanitizer to detect memory error" OFF)


if(BUILD_ASAN AND BUILD_UBSAN)
    message(FATAL_ERROR "Can't enable AddressSanitizer(BUILD_ASAN=ON) and "
        "UndefinedBehaviorSanitizer(BUILD_UBSAN=ON) concurrently")
endif()
if(BUILD_ASAN)
    message("-- AddressSanitizer WILL be compiled in as BUILD_ASAN=ON")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
else()
    message("-- AddressSanitizer will NOT be compiled in as BUILD_ASAN=OFF")
endif()

if(BUILD_MSAN)
    if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "MemorySanitizer is supported by clang only, but "
            "current compiler is [${CMAKE_C_COMPILER_ID}]")
    endif()
    message("-- MemorySanitizer WILL be compiled in as BUILD_MSAN=ON")
    add_compile_options(-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=memory)
else()
    message("-- MemorySanitizer will NOT be compiled in as BUILD_MSAN=OFF")
endif()

if(BUILD_UBSAN)
    message("-- UndefinedBehaviorSanitizer WILL be compiled in as BUILD_UBSAN=ON")
    add_compile_options(-fsanitize=undefined -g)
    add_link_options(-fsanitize=undefined)
else()
    message("-- UndefinedBehaviorSanitizer will NOT be compiled in as BUILD_UBSAN=OFF")
endif()



add_compile_options(-Wall -Wextra -pedantic -O1)



add_subdirectory(src/mycrypto)
add_subdirectory(tools)
add_subdirectory(test)

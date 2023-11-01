cmake_minimum_required(VERSION 3.18.4)

project(LIBMYCRYPTO)

# Always generate compile_commands.json for clangd, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/compiler-flags.cmake)
include(cmake/sanitizers.cmake)

add_subdirectory(src/mycrypto)
add_subdirectory(tools)
add_subdirectory(tests)

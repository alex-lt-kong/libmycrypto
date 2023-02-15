cmake_minimum_required(VERSION 3.18.4)


project(LIBMYCRYPTO)

add_compile_options(-Wall -Wextra -pedantic -O1)

#add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
#add_link_options(-fsanitize=address)

add_subdirectory(src/mycrypto)
add_subdirectory(tools)
add_subdirectory(test)

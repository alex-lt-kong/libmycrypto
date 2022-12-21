cmake_minimum_required(VERSION 3.18.4)

project(LIBMYCRYPTO)

add_compile_options(-Wall -Wextra -pedantic -O3)

find_library(CRITERION_LIB criterion)
if(NOT CRITERION_LIB)
  message(FATAL_ERROR "criterion library not found")
endif()

add_executable(test-sha ./test/sha/test-sha.c)
add_executable(test-ripemd ./test/ripemd/test-ripemd.c)
add_executable(test-hmac ./test/hmac/test-hmac.c)
add_executable(test-base32-64 ./test/base/test-base32-64.c)
add_executable(test-base58 ./test/base/test-base58.c)

add_subdirectory(src)
add_subdirectory(tools)

target_link_libraries(test-sha       mycrypto criterion)
target_link_libraries(test-ripemd    mycrypto criterion)
target_link_libraries(test-hmac      mycrypto criterion)
target_link_libraries(test-base32-64 mycrypto criterion m)
target_link_libraries(test-base58    mycrypto criterion)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/sha/SHA256LongMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/sha/SHA256ShortMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/sha/SHA1LongMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/sha/SHA1ShortMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

add_custom_command(TARGET test-sha POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test-sha --verbose
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running SHA test..."
)

add_custom_command(TARGET test-hmac POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test-hmac --verbose
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running HMAC test..."
)

add_custom_command(TARGET test-base32-64 POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test-base32-64 --verbose
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Base32/64 test..."
)

add_custom_command(TARGET test-base58 POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test-base58 --verbose
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Base58 test..."
)

add_custom_command(TARGET test-ripemd POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test-ripemd --verbose
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running RIPEMD test..."
)
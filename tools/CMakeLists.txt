add_executable(base64-encode base64-encode.c)
add_executable(base64-decode base64-decode.c)

target_link_libraries(base64-encode mycrypto)
target_link_libraries(base64-decode mycrypto)
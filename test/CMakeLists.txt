add_executable(test-sha ./sha/test-sha.c)
add_executable(test-ripemd ./ripemd/test-ripemd.c)
add_executable(test-hmac ./hmac/test-hmac.c)
add_executable(test-base32-64 ./base/test-base32-64.c)
add_executable(test-base58 ./base/test-base58.c)

target_link_libraries(test-sha       mycrypto)
target_link_libraries(test-ripemd    mycrypto criterion)
target_link_libraries(test-hmac      mycrypto criterion)
target_link_libraries(test-base32-64 mycrypto criterion m)
target_link_libraries(test-base58    mycrypto criterion)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sha/SHA256LongMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sha/SHA256ShortMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sha/SHA1LongMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sha/SHA1ShortMsg.rsp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)


add_custom_target(test)
add_custom_command(TARGET test COMMAND test-ripemd --verbose COMMENT "Running ripemd test...")
add_custom_command(TARGET test COMMAND test-sha --verbose COMMENT "Running SHA test...")
add_custom_command(TARGET test COMMAND test-base58 --verbose  COMMENT "Running Base58 test...")
add_custom_command(TARGET test COMMAND test-base32-64 --verbose  COMMENT "Running Base32/64 test...")
add_custom_command(TARGET test COMMAND test-hmac --verbose COMMENT "Running HMAC test...")

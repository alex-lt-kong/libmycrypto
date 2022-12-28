set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(sha256 sha256.c)
add_library(sha1 sha1.c)
add_library(hmac hmac.c)
add_library(base32 base32.c)
add_library(base64 base64.c)
add_library(base58 base58.c)
add_library(ripemd160 ripemd160.c)
add_library(misc misc.c)

add_library(mycrypto sha256 sha1 hmac base32 base58 base64 ripemd160 misc)
target_link_libraries(mycrypto)

set_target_properties(mycrypto PROPERTIES PUBLIC_HEADER "sha256.h;sha1.h;hmac.h;base32.h;base58.h;base64.h;ripemd160.h;misc.h;")

install(TARGETS mycrypto 
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/mycrypto
)

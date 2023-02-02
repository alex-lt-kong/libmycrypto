# libmycrypto

* Yet another cryptography library implemented in ISO C for study and general
usage with a focus on cryptocurrency applications such as
[libmybitcoin](https://github.com/alex-lt-kong/libmybitcoin).
  * It could be more straightforwardly named just "libcrypto". This name,
  unfortunately, has been occupied by the OpenSSL crypto library. Therefore,
  here let's use "libmycrypto" instead.

* Available algorithms are: Base32, Base58, Base64, HMAC-SHA1, HMAC-SHA256,
RIPEMD160, SHA1, SHA256.
  * All algorithms are tested against either official and/or well established
  test vectors.

* Should work on Linux and Windows (MinGW) with gcc and clang.

## Dependencies

* `cmake` is used to generate `Makefile`: `apt install cmake`.

## Build and install

### Linux
* Build: 
  ```shell
  mkdir -p ./build
  cd ./build
  cmake ../
  make
  ```
* Make it globally available: `sudo make install`.
* Run all tests: `make test`.

### Windows (vcpkg+MinGW)

* Build:
  ```shell
  mkdir -p ./build
  cd ./build
  cmake ../ "-DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake" -G "MinGW Makefiles"
  mingw32-make.exe
  ```

* Run all tests: `mingw32-make.exe test`.

## Test results

| Algorithm     | Last Tested On  | Test Vectors From|
| ------------- | -------------   | ------------- |
| Base32        | 2023-01-12      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| Base58        | 2023-01-12      | [draft-msporny-base58-03](https://datatracker.ietf.org/doc/html/draft-msporny-base58-03#section-5)<br />[Bitcoin Core](https://github.com/bitcoin/bitcoin/blob/master/src/test/data/base58_encode_decode.json)|
| Base64        | 2023-01-12      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)<br />[BoringSSL from Google](https://boringssl.googlesource.com/boringssl/+/master/crypto/base64/base64_test.cc)
| HMAC-SHA1     | 2023-01-12      | [RFC 2202](https://datatracker.ietf.org/doc/html/rfc2202)|
| HMAC-SHA256   | 2023-01-12      | [RFC 4231](https://datatracker.ietf.org/doc/html/rfc4231)|
| RIPEMD160     | 2023-01-12      | [The RIPEMD-160 homepage](https://homes.esat.kuleuven.be/~bosselae/ripemd160.html)|
| SHA1          | 2023-01-12      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|
| SHA256        | 2023-01-12      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|


## Interoperability
### C++

* As C++ is mostly a superset of C, using this project does not need any extra
modification. However, if we would like to take advantage of some of C++'s
more "modern" features, we may want to do things a bit differently.

* For the sake of convenience, two zero-cost wrappers on top of
`std::unique_ptr` (`unique_byte_ptr` and `unique_char_ptr`) are prepared
in [misc.hpp](./src/mycrypto/misc.hpp).

    * For example, we do:
    ```C++
    unique_byte_ptr bytes(hex_string_to_bytes(hex_cstr, &input_bytes_len));
    return;
    ```
    instead of
    ```C
    uint8_t* bytes = hex_string_to_bytes(hex_cstr, &input_bytes_len);
    free(bytes);
    return;
    ```
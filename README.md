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

* Should work on Unix-like platforms and Windows (MinGW) with gcc and clang.

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
| Base32        | 2023-02-12      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| Base58        | 2023-02-12      | [draft-msporny-base58-03](https://datatracker.ietf.org/doc/html/draft-msporny-base58-03#section-5)<br />[Bitcoin Core](https://github.com/bitcoin/bitcoin/blob/master/src/test/data/base58_encode_decode.json)|
| Base64        | 2023-02-12      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)<br />[BoringSSL from Google](https://boringssl.googlesource.com/boringssl/+/master/crypto/base64/base64_test.cc)
| HMAC-SHA1     | 2023-02-12      | [RFC 2202](https://datatracker.ietf.org/doc/html/rfc2202)|
| HMAC-SHA256   | 2023-02-12      | [RFC 4231](https://datatracker.ietf.org/doc/html/rfc4231)|
| RIPEMD160     | 2023-02-12      | [The RIPEMD-160 homepage](https://homes.esat.kuleuven.be/~bosselae/ripemd160.html)|
| SHA1          | 2023-02-12      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|
| SHA256        | 2023-02-12      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|


## Interoperability
### C++

* As C++ is mostly a superset of C, using this project does not need any extra
modification. However, if we would like to take advantage of some of C++'s
more "modern" features, we may want to do things a bit differently.

* For the sake of convenience, `unique_fptr`, a "zero-cost" (
  [sort of](https://github.com/alex-lt-kong/the-nitty-gritty/tree/main/cpp/10_smart-pointers#is-unique_ptr-a-zero-cost-wrapper-on-top-of-raw-pointer))
  wrapper on top of `std::unique_ptr` (i.e., a unique_ptr with a `free()`
  deleter) is prepared in
[misc.hpp](./src/mycrypto/misc.hpp).

    * For example, we do:

      ```C++
      unique_fptr<uint8_t[]> bytes(hex_string_to_bytes(hex_cstr, &input_bytes_len));
      return;
      ```

      in C++ to enjoy the benefit of RAII instead of

      ```C
      uint8_t* bytes = hex_string_to_bytes(hex_cstr, &input_bytes_len);
      free(bytes);
      return;
      ```

      in C.

## Quality assurance

### Sanitizers

* Instead of `cmake ../`,
    * run `cmake .. -DBUILD_ASAN=ON` then `make test` to test memory error with
[AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer).
    * run `cmake ../ -DBUILD_MSAN=ON` then `make test` to test the library with
[MemorySanitizer](https://github.com/google/sanitizers/wiki/MemorySanitizer).
Note that this test supports `clang` only.
    * run `cmake ../ -DBUILD_UBSAN=ON` then `make test` to test the library with
[UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html).
* The repo is also frequently tested with `Valgrind`: `valgrind --leak-check=yes --log-file=valgrind.rpt ./test/test-hmac`. Unfortunately, this part is not automated.

### Fuzz testing

* The repo is tested with AFL++.

* Build with instrumentation
```
cmake -DCMAKE_C_COMPILER=afl-clang-fast \
      -DCMAKE_C_FLAGS="-frtti -fsanitize=undefined -fno-sanitize-recover=all -g" \
      -DCMAKE_EXE_LINKER_FLAGS=" -frtti -fsanitize=undefined -fno-sanitize-recover=all" \
      -DCMAKE_MODULE_LINKER_FLAGS="-frtti -fsanitize=undefined -fno-sanitize-recover=all" \
    ..
```

* Go: `afl-fuzz -i ../tests/afl++/inputs/ -o  /tmp/outputs/ ./tests/fuzz-base --encoding-scheme 32 --test-case-path @@`
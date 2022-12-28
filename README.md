# libmycrypto

* Yet another cryptography library implemented in ISO C for study and general usage with a focus
on cryptocurrency applications such as [bitcoin-internals](https://github.com/alex-lt-kong/bitcoin-internals).
  * It could be more straightforwardly named just "libcrypto". This name, unfortunately, has been occupied by
    the OpenSSL crypto library. Therefore, here let's use "libmycrypto" instead.

* Available algorithms are: Base32, Base58, Base64, HMAC-SHA1, HMAC-SHA256, RIPEMD160, SHA1, SHA256.
  * All algorithms are tested against either official and/or well established test vectors.

* Should work on Linux and Windows with gcc, clang and MSVC.

## Dependencies

* `cmake` is used to generate `Makefile`: `apt install cmake`.

## Build and install

* Build: 
  ```
  mkdir -p ./build
  cd ./build
  cmake ../
  make
  ```
* Make it globally available: `sudo make install`.
* Run all tests: `make test`.

## Test results

| Algorithm     | Last Tested On  | Test Vectors From|
| ------------- | -------------   | ------------- |
| Base32        | 2022-12-20      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| Base58        | 2022-12-20      | [draft-msporny-base58-03](https://datatracker.ietf.org/doc/html/draft-msporny-base58-03#section-5)<br />[Bitcoin Core](https://github.com/bitcoin/bitcoin/blob/master/src/test/data/base58_encode_decode.json)|
| Base64        | 2022-12-20      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)<br />[BoringSSL from Google](https://boringssl.googlesource.com/boringssl/+/master/crypto/base64/base64_test.cc)
| HMAC-SHA1     | 2022-12-20      | [RFC 2202](https://datatracker.ietf.org/doc/html/rfc2202)|
| HMAC-SHA256   | 2022-12-20      | [RFC 4231](https://datatracker.ietf.org/doc/html/rfc4231)|
| RIPEMD160     | 2022-12-20      | [The RIPEMD-160 homepage](https://homes.esat.kuleuven.be/~bosselae/ripemd160.html)|
| SHA1          | 2022-12-20      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|
| SHA256        | 2022-12-20      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|

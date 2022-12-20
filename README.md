# libmycrypto

* A few cryptographic algorithms implemented in ISO C for study and general usage with a focus
on cryptocurrency applications such as [bitcoin-internals](https://github.com/alex-lt-kong/bitcoin-internals).
  * It could be more straightforwardly named just "libcrypto". This name, unfortunately, has been occupied by
    the OpenSSL crypto library. Therefore, here let's use "libmycrypto" instead.

* Available algorithms are: Base32, Base58, Base64, HMAC-SHA1, HMAC-SHA256, RIPEMD160, SHA1, SHA256.  

## Build

* Criterion is used for unit test: `apt install libcriterion-dev libcriterion3`

* `mkdir -p ./build && cd ./build`, `cmake ../ && make`

* To make it globally available: `sudo make install`

## Test results

| Algorithm     | Last Tested On  | Test Vectors From|
| ------------- | -------------   | ------------- |
| Base32        | 2022-12-19      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| Base58        | 2022-12-19      | [draft-msporny-base58-03](https://datatracker.ietf.org/doc/html/draft-msporny-base58-03#section-5)<br />[Bitcoin Core](https://github.com/bitcoin/bitcoin/blob/master/src/test/data/base58_encode_decode.json)|
| Base64        | 2022-12-19      | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| HMAC-SHA1     | 2022-12-19      | [RFC 2202](https://datatracker.ietf.org/doc/html/rfc2202)|
| HMAC-SHA256   | 2022-12-19      | [RFC 4231](https://datatracker.ietf.org/doc/html/rfc4231)|
| RIPEMD160     | 2022-12-19      | [The RIPEMD-160 homepage](https://homes.esat.kuleuven.be/~bosselae/ripemd160.html)|
| SHA1          | 2022-12-19      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|
| SHA256        | 2022-12-19      | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|

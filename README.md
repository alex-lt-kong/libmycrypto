# cryptographic-algorithms

* CUnit is used for unit test: `apt install libcunit1 libcunit1-doc libcunit1-dev`

* A few cryptographic algorithms for study and general usage with a focus on cryptocurrency applications.

* Available algorithms are: Base32, Base58, Base64, HMAC-SHA1, HMAC-SHA256, RIPEMD160, SHA1, SHA256.

* Test results

| Algorithm     | Last Tested On  | Test Results At|Test Vectors From|
| ------------- | -------------   | ------------- |------------- |
| Base32        | 2022-09-07      | [./tests/base/README.md](./tests/base/README.md)     | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| Base64        | 2022-09-07      | [./tests/base/README.md](./tests/base/README.md)     | [RFC 4648](https://datatracker.ietf.org/doc/html/rfc4648#section-10)|
| HMAC-SHA1     | 2022-07-24      | [./tests/hmac/README.md](./tests/hmac/README.md)     | [RFC 2202](https://datatracker.ietf.org/doc/html/rfc2202)|
| HMAC-SHA256   | 2022-07-24      | [./tests/hmac/README.md](./tests/hmac/README.md)     | [RFC 4231](https://datatracker.ietf.org/doc/html/rfc4231)|
| RIPEMD160     | 2022-07-24      | [./tests/ripemd/README.md](./tests/ripemd/README.md) | [The RIPEMD-160 homepage](https://homes.esat.kuleuven.be/~bosselae/ripemd160.html)|
| SHA1          | 2022-12-06      | [./tests/sha/test.output](./tests/sha/test.output)     | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|
| SHA256        | 2022-12-06      | [./tests/sha/test.output](./tests/sha/test.output)     | [NIST](https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Secure-Hashing)|


#include <assert.h>
#include <fstream>
#include <limits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../hmac.h"
#include "../../sha1.h"
#include "../../sha256.h"
#include "../../misc.h"

using namespace std;

static_assert (CHAR_BIT == 8);

int main() {
  unsigned char key[] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
  };
  unsigned char data[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f }; // Hello
  unsigned char sha1_out[SHA1_HASH_SIZE];
  hmac_sha1(key, sizeof(key), data, sizeof(data), sha1_out);
  cout << "Function result: "<< bytes_to_hex_string(sha1_out, SHA1_HASH_SIZE) << endl;
  cout << "Expected result: 24dfd13fb32a4a50a3962997787970c3e0d77fe4" << endl;
  unsigned char sha256_out[SHA256_HASH_SIZE];
  hmac_sha256(key, sizeof(key), data, sizeof(data), sha256_out);
  cout << "Function Result: "<< bytes_to_hex_string(sha256_out, SHA256_HASH_SIZE) << endl;
  cout << "Expected result: 183273bbe9800d87a9a600b8c5232661a9ddb312df6f431d634f1db2c296cd32" << endl;
  return 0;
}

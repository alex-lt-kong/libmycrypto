#include <iostream>
#include <string.h>

#include "../../src/base58.h"

using namespace std;

int main() {
  uint8_t input[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
  // Hello World!
  //uint8_t input[] = "The quick brown fox jumps over the lazy dog.";
  // Answer: USm3fpXnKG5EUBx2ndxBDMPVciP5hGey2Jh4NDv6gmeo1LkMeiKrLJUUBk6Z
  char* output = (char*)calloc(1024, 1);
  size_t output_len = 512;
  bool res = b58enc(input, sizeof(input), output, &output_len);
  cout << "Result:" << res << "\nLength: " << output_len << endl;
  for (int i = 0; i < output_len; i++) {
    cout << output[i];
  }
  //delete output_len;
  cout << endl;
  
  /*uint8_t* new_output;
  size_t* new_output_len = (size_t*)malloc(sizeof(size_t));

  new_output = decode_base64_string_to_bytes(output, *output_len, new_output_len);
  for (int i = 0; i < *new_output_len; i++) {
    cout << new_output[i];
  }*/
  return 0;
}

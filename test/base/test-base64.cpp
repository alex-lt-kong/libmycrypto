#include <iostream>

#include "../../base64.h"

using namespace std;

int main() {
  unsigned char input[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x21, 0x21};
  char* output;
  size_t* output_len = (size_t*)malloc(sizeof(size_t));
  output = encode_bytes_to_base64_string(input, sizeof(input), output_len);
  for (int i = 0; i < *output_len-1; i++) {
    cout << output[i];
  }
  cout << endl;

  unsigned char* new_output;
  size_t* new_output_len = (size_t*)malloc(sizeof(size_t));

  new_output = decode_base64_string_to_bytes(output, *output_len, new_output_len);
  for (int i = 0; i < *new_output_len; i++) {
    cout << new_output[i];
  }
  return 0;
}

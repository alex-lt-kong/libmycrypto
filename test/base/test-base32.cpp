#include <iostream>

#include "../../base32.h"

using namespace std;

int main() {
  unsigned char input[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x21, 0x21};
  char* output;
  size_t* output_len;
  
  output = b32_encode(input, sizeof(input), output_len);
  for (int i = 0; i < *output_len; i++) {
    cout << output[i];
  }
  //delete output_len;
  cout << endl;
  
  /*unsigned char* new_output;
  size_t* new_output_len = (size_t*)malloc(sizeof(size_t));

  new_output = decode_base64_string_to_bytes(output, *output_len, new_output_len);
  for (int i = 0; i < *new_output_len; i++) {
    cout << new_output[i];
  }*/
  return 0;
}

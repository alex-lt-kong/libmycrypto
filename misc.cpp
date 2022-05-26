#include <iostream>
#include <sstream>
#include <iomanip>

#include "misc.h"

using namespace std;

string bytes_to_hex_string(const unsigned char* input, const size_t input_len) {

  stringstream ss;

  ss << hex;
  for(int i = 0; i < input_len; i++) {
      ss << hex << setw(2) << std::setfill('0')<< (int)input[i];
  }
  return ss.str();
}

void hex_string_to_bytes(const string& hex_input, unsigned char* output) {

  string hex_input_ = hex_input;

  if (hex_input_.length() % 2 != 0) {
      hex_input_.insert(0, "0");
  }
  
  for (int i = 0; i < hex_input_.length(); i += 2) {
    output[i/2] = strtol(hex_input_.substr(i, 2).c_str(), NULL, 16);
  }
}

#include <assert.h>
#include <fstream>
#include <limits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../sha256.h"
#include "../../sha1.h"
#include "../../misc.h"

using namespace std;

static_assert (CHAR_BIT == 8);

bool test_rsp_file(
  string rsp_file_dir, void (*hash_func)(const unsigned char*, size_t input_len, unsigned char*), const size_t hash_size
  ) {
  bool all_passed = true;
  ifstream infile (rsp_file_dir, ifstream::in);
  string line;
  unsigned char sha_hash[hash_size];
  size_t msg_len = -1;
  string msg;
  char* md_char;
  while (getline(infile, line)) {
    
    if (line.length() >= 3 && line.substr(0, 3) == "Len") {
      msg_len = stoi(line.substr(6, line.length() - 6 - 1));      
    } else if (line.length() >= 3 && line.substr(0, 3) == "Msg") {
      msg = line.substr(6, line.length() - 6 - 1);
      assert (msg.length() % 2 == 0);
    } else if (line.length() >= 2 && line.substr(0, 2) == "MD") {
      
      string official_md = line.substr(5, line.length() - 5 - 1);
      
      cout << "Original Message: " << msg.substr(0, 128) << (msg.length() > 128 ? "...[Truncated]" : "" ) << "\n";
      unsigned char msg_bytes[msg_len / CHAR_BIT];            
      hex_string_to_bytes(msg.c_str(), msg_bytes, strlen(msg.c_str()));
      hash_func(msg_bytes, msg_len / CHAR_BIT, sha_hash);
      md_char = bytes_to_hex_string(sha_hash, hash_size, false);
      printf("Function result:  %s\n", md_char);
      cout << "Expected result:  " << official_md << " ";
      if (strcmp(md_char, official_md.c_str()) == 0) {
        cout << "Passed" << endl;
      } else {
        cout << "!!!FAILED!!!" << endl;
        all_passed = false;
      }
      free(md_char);
    }
  }
  infile.close();
  return all_passed;
}


int main() {
  bool all_passed = true;
  cout << "========== Testing SHA256 ==========" << endl;
  all_passed &= test_rsp_file("../../test-vectors/SHA256ShortMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
  all_passed &= test_rsp_file("../../test-vectors/SHA256LongMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
  cout << "\n\n========== Testing SHA1 ==========" << endl;
  all_passed &= test_rsp_file("../../test-vectors/SHA1ShortMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
  all_passed &= test_rsp_file("../../test-vectors/SHA1LongMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
  if (all_passed) {
    cout << "\n\n========== ALL tests passed ==========" << endl; 
  } else {
    cout << "\n\n========== FAILED to pass some tests ==========" << endl; 
  }

  return 0;
}

#include <assert.h>
#include <fstream>
#include <limits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../sha256.h"
#include "../../misc.h"

using namespace std;

static_assert (CHAR_BIT == 8);

void test_rsp_file(string rsp_file_dir) {
  ifstream infile (rsp_file_dir, ifstream::in);
  string line;
  unsigned char sha_hash[SHA256_HASH_SIZE];
  size_t next_msg_len = -1;
  char md_char[SHA256_HASH_SIZE];
  while (getline(infile, line)) {
    if (line.length() >= 3 && line.substr(0, 3) == "Len") {
      next_msg_len = stoi(line.substr(6, line.length() - 6 - 1));
      cout << "Len = " << next_msg_len << "\n";
    } else if (line.length() >= 3 && line.substr(0, 3) == "Msg") {
      string msg = line.substr(6, line.length() - 6 - 1);
      assert (msg.length() % 2 == 0);
      cout << "Msg = " << msg.substr(0, 128) << (msg.length() > 128 ? "...[Truncated]" : "" ) << "\n";
      unsigned char msg_bytes[next_msg_len / CHAR_BIT];            
      HexToBin(msg.c_str(), msg_bytes, strlen(msg.c_str()));
      cal_sha256_hash(msg_bytes, next_msg_len / CHAR_BIT, sha_hash);     
      
      BinToHex(sha_hash, SHA256_HASH_SIZE, md_char, SHA256_HASH_SIZE * 2 + 1, 0);
      printf("MD = %s ", md_char);
    } else if (line.length() >= 2 && line.substr(0, 2) == "MD") {
      string official_md = line.substr(5, line.length() - 5 - 1);
      if (strcmp(md_char, official_md.c_str()) == 0) {
        cout << "Passed" << endl;
      } else {
        cout << "====== FAILED ======, expecting " << official_md << endl;
      }
    }
  }
  infile.close();
}

int main() {
  test_rsp_file("../../test-vectors/SHA256ShortMsg.rsp");
  test_rsp_file("../../test-vectors/SHA256LongMsg.rsp");
  return 0;
}

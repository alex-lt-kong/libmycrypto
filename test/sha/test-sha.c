#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../../src/sha256.h"
#include "../../src/sha1.h"
#include "../../src/misc.h"

void test_rsp_file(
  char* rsp_file_dir, void (*hash_func)(const uint8_t*, size_t input_len, uint8_t*), const size_t hash_size
) {

  FILE *fp;
  fp = fopen(rsp_file_dir, "r");
  if(fp == NULL) {
    cr_fail("fopen() failed\n");
  }
  char line[PATH_MAX] = {0};
  uint8_t sha_hash[hash_size];
  uint8_t* msg_bytes;
  size_t msg_len = -1;
  size_t msg_bytes_len = -1;
  char msg[PATH_MAX] = {0};
  char official_md[65] = {0};
  char* md_char;
  while (fgets(line, PATH_MAX, fp) != NULL) {

    if (strlen(line) >= 3 && strncmp(line, "Len", 3) == 0) {
      msg_len = strtol(line + 6, NULL, 10);
    } else if (strlen(line) >= 3 && strncmp(line, "Msg", 3) == 0) {
      memcpy(msg, line + 6, strlen(line) - 6 - 2);
      // rsp file's lines end with LFCR so it is two-byte long...
    } else if (strlen(line) >= 2 && strncmp(line, "MD", 2) == 0) {      
      memcpy(official_md, line + 5, strlen(line) - 5 - 2);

      msg_bytes = hex_string_to_bytes(msg, &msg_bytes_len);
      if (msg_bytes == NULL && msg_bytes_len > 0) {
        return;
      }
      hash_func(msg_bytes, msg_len / CHAR_BIT, sha_hash);
      md_char = bytes_to_hex_string(sha_hash, hash_size, false);
      if (md_char == NULL) {
        cr_fail("bytes_to_hex_string() failed\n");
      }
      cr_expect(eq(str, md_char, official_md));
      free(md_char);
      free(msg_bytes);
      memset(msg, 0, sizeof(msg));
      memset(official_md, 0, sizeof(official_md));
    }
    memset(line, 0, sizeof(line));   
  }
  fclose(fp);
}

Test(test_sha_suite, test_sha256) {
  test_rsp_file("SHA256ShortMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
  test_rsp_file("SHA256LongMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
}


Test(test_sha_suite, test_sha1) {
  test_rsp_file("SHA1ShortMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
  test_rsp_file("SHA1LongMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
}

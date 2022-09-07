#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../src/sha256.h"
#include "../../src/sha1.h"
#include "../../src/misc.h"

bool test_rsp_file(
  char* rsp_file_dir, void (*hash_func)(const uint8_t*, size_t input_len, uint8_t*), const size_t hash_size
  ) {
  bool all_passed = true;

  FILE *fp;
  fp = fopen(rsp_file_dir, "r");
  if(fp == NULL) {
    perror("Error opening file");
    return false;
  }
  char line[65536] = {0};
  uint8_t sha_hash[hash_size];
  uint8_t* msg_bytes;
  size_t msg_len = -1;
  size_t msg_bytes_len = -1;
  char msg[65536] = {0};
  char official_md[65] = {0};
  char* md_char;
  int count = 0;
  while (fgets(line, 65536, fp) != NULL) {

    if (strlen(line) >= 3 && strncmp(line, "Len", 3) == 0) {
      msg_len = strtol(line + 6, NULL, 10);
    } else if (strlen(line) >= 3 && strncmp(line, "Msg", 3) == 0) {
      memcpy(msg, line + 6, strlen(line) - 6 - 2);
      // rsp file's lines end with LFCR so it is two-byte long...
    } else if (strlen(line) >= 2 && strncmp(line, "MD", 2) == 0) {      
      memcpy(official_md, line + 5, strlen(line) - 5 - 2);
      
      printf("Original Message: [");
      for (int i = 0; i < strlen(msg); ++i) {
        printf("%c", msg[i]);
        if (i >= 64) {
          printf("...<Truncated>");
          break;
        }
      }
      printf("]\n");

      msg_bytes = hex_string_to_bytes(msg, &msg_bytes_len);
      hash_func(msg_bytes, msg_len / CHAR_BIT, sha_hash);
      md_char = bytes_to_hex_string(sha_hash, hash_size, false);
      printf("Function result:  [%s]\n", md_char);
      printf("Expected result:  [%s] ", official_md);
      if (strcmp(md_char, official_md) == 0) {
        printf("Passed\n\n");
      } else {
        printf("!!!FAILED!!!\n\n");
        all_passed = false;
      }
      free(md_char);
      free(msg_bytes);
      memset(msg, 0, sizeof(msg));
      memset(official_md, 0, sizeof(official_md));
    }
    memset(line, 0, sizeof(line));   
  }
  fclose(fp);
  return all_passed;
}


int main() {
  
  freopen("README.md", "w", stdout); // seems we don't need to close() an freopen()'ed file.
  printf("```\n");
  time_t now;
  time(&now);
  char utc_time_str[sizeof "1970-01-01T00:00:00Z"];
  strftime(utc_time_str, sizeof(utc_time_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
  printf("Tests start at %s\n\n\n", utc_time_str);

  bool all_passed = true;
  printf("========== Testing SHA256 ==========\n");
  all_passed &= test_rsp_file("SHA256ShortMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
  all_passed &= test_rsp_file("SHA256LongMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
  printf("\n\n========== Testing SHA1 ==========\n");
  all_passed &= test_rsp_file("SHA1ShortMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
  all_passed &= test_rsp_file("SHA1LongMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
  if (all_passed) {
    printf("\n\n========== ALL tests passed ==========\n"); 
  } else {
    printf("\n\n========== FAILED to pass some tests ==========\n"); 
  }

  return 0;
}

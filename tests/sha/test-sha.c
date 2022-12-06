#include <assert.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../src/sha256.h"
#include "../../src/sha1.h"
#include "../../src/misc.h"

int init_suite_success(void) { return 0; }
int init_suite_failure(void) { return -1; }
int clean_suite_success(void) { return 0; }
int clean_suite_failure(void) { return -1; }

bool test_rsp_file(
  char* rsp_file_dir, void (*hash_func)(const uint8_t*, size_t input_len, uint8_t*), const size_t hash_size
) {

  FILE *fp;
  fp = fopen(rsp_file_dir, "r");
  if(fp == NULL) {
    perror("Error opening file");
    return false;
  }
  char line[PATH_MAX] = {0};
  uint8_t sha_hash[hash_size];
  uint8_t* msg_bytes;
  size_t msg_len = -1;
  size_t msg_bytes_len = -1;
  char msg[PATH_MAX] = {0};
  char official_md[65] = {0};
  char* md_char;
  int count = 0;
  while (fgets(line, PATH_MAX, fp) != NULL) {

    if (strlen(line) >= 3 && strncmp(line, "Len", 3) == 0) {
      msg_len = strtol(line + 6, NULL, 10);
    } else if (strlen(line) >= 3 && strncmp(line, "Msg", 3) == 0) {
      memcpy(msg, line + 6, strlen(line) - 6 - 2);
      // rsp file's lines end with LFCR so it is two-byte long...
    } else if (strlen(line) >= 2 && strncmp(line, "MD", 2) == 0) {      
      memcpy(official_md, line + 5, strlen(line) - 5 - 2);
      
      /*printf("Original Message: [");
      for (int i = 0; i < strlen(msg); ++i) {
        printf("%c", msg[i]);
        if (i >= 64) {
          printf("...<Truncated>");
          break;
        }
      }
      printf("]\n");*/

      msg_bytes = hex_string_to_bytes(msg, &msg_bytes_len);
      if (msg_bytes == NULL) {
        return false;
      }
      hash_func(msg_bytes, msg_len / CHAR_BIT, sha_hash);
      md_char = bytes_to_hex_string(sha_hash, hash_size, false);
      if (md_char == NULL) {
        return false;
      }
      //printf("Function result:  [%s]\n", md_char);
      //printf("Expected result:  [%s] ", official_md);
      CU_ASSERT_STRING_EQUAL(md_char, official_md);

      free(md_char);
      free(msg_bytes);
      memset(msg, 0, sizeof(msg));
      memset(official_md, 0, sizeof(official_md));
    }
    memset(line, 0, sizeof(line));   
  }
  fclose(fp);
  return true;
}

void test_sha256_short_msg(void) {
  CU_ASSERT_TRUE(test_rsp_file("SHA256ShortMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE));
}

void test_sha256_long_msg(void) {
  test_rsp_file("SHA256LongMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE);
}

void test_sha1_short_msg(void) {
  test_rsp_file("SHA1ShortMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
}

void test_sha1_long_msg(void) {
  test_rsp_file("SHA1LongMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE);
}

int main() {

  CU_pSuite pSuite = NULL;
  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite_SHA", init_suite_success, clean_suite_success);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if (
    (NULL == CU_add_test(pSuite, "test_sha256_short_msg", test_sha256_short_msg)) ||
    (NULL == CU_add_test(pSuite, "test_sha256_long_msg", test_sha256_long_msg)) ||
    (NULL == CU_add_test(pSuite, "test_sha1_short_msg", test_sha1_short_msg)) ||
    (NULL == CU_add_test(pSuite, "test_sha1_long_msg", test_sha1_long_msg))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  
  CU_basic_show_failures(CU_get_failure_list());

  CU_cleanup_registry();
  return CU_get_error();
}

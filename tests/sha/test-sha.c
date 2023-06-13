#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mycrypto/misc.h"
#include "mycrypto/sha1.h"
#include "mycrypto/sha256.h"

#define BUF_SIZE 65536

int test_rsp_file(char *rsp_file_dir,
                  void (*hash_func)(const uint8_t *, size_t input_len,
                                    uint8_t *),
                  const size_t hash_size) {

  FILE *fp;
  fp = fopen(rsp_file_dir, "r");
  if (fp == NULL) {
    fprintf(stderr, "fopen() failed\n");
    return 1;
  }
  char line[BUF_SIZE] = {0};
  uint8_t *sha_hash = malloc(hash_size * sizeof(uint8_t));
  uint8_t *msg_bytes;
  size_t msg_len = -1;
  int64_t msg_bytes_len = -1;
  char msg[BUF_SIZE] = {0};
  char official_md[SHA256_HASH_SIZE * 2 + 3] = {0};
  char *md_char;
  while (fgets(line, BUF_SIZE, fp) != NULL) {

    if (strlen(line) >= 3 && strncmp(line, "Len", 3) == 0) {
      msg_len = strtol(line + 6, NULL, 10);
    } else if (strlen(line) >= 3 && strncmp(line, "Msg", 3) == 0) {
      memcpy(msg, line + 6, strlen(line) - 6);
      /* rsp file's lines end with 0x0d,0x0a (i.e., CRLF) so it is
      two-byte long. However, the handling of CRLF could be different
      on Linux and Windows, making the below loop necessary
       */
      while (msg[strlen(msg) - 1] == 0x0d || msg[strlen(msg) - 1] == 0x0a) {
        msg[strlen(msg) - 1] = '\0';
      }
    } else if (strlen(line) >= 2 && strncmp(line, "MD", 2) == 0) {
      memcpy(official_md, line + 5, strlen(line) - 5);

      while (official_md[strlen(official_md) - 1] == 0x0d ||
             official_md[strlen(official_md) - 1] == 0x0a) {
        official_md[strlen(official_md) - 1] = '\0';
      }
      msg_bytes = hex_string_to_bytes(msg, &msg_bytes_len);
      if ((msg_bytes == NULL && msg_bytes_len > 0)) {
        return 0;
      }
      hash_func(msg_bytes, msg_len / CHAR_BIT, sha_hash);
      md_char = bytes_to_hex_string(sha_hash, hash_size, false);
      if (md_char == NULL) {
        fprintf(stderr, "bytes_to_hex_string() failed\n");
        return 1;
      }
      if (strcmp(md_char, official_md) != 0) {
        fprintf(stderr, "FAILED\nExpect: %s\nActual: %s\n", official_md,
                md_char);
        return 1;
      }
      free(md_char);
      free(msg_bytes);
      memset(msg, 0, sizeof(msg));
      memset(official_md, 0, sizeof(official_md));
    }
    memset(line, 0, sizeof(line));
  }
  fclose(fp);
  free(sha_hash);
  return 0;
}

int main() {
  int retval = 0;

  printf("Testing cal_sha256_hash() with SHA256ShortMsg.rsp\n");
  if (test_rsp_file("SHA256ShortMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE) !=
      0) {
    fprintf(stderr, "FAILED!!!\n");
    ++retval;
  }

  printf("Testing cal_sha256_hash() with SHA256LongMsg.rsp\n");
  if (test_rsp_file("SHA256LongMsg.rsp", &cal_sha256_hash, SHA256_HASH_SIZE) !=
      0) {
    fprintf(stderr, "FAILED!!!\n");
    ++retval;
  }

  printf("Testing cal_sha1_hash() with SHA1ShortMsg.rsp\n");
  if (test_rsp_file("SHA1ShortMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE) != 0) {
    fprintf(stderr, "FAILED!!!\n");
    ++retval;
  }

  printf("Testing cal_sha1_hash() with SHA1LongMsg.rsp\n");
  if (test_rsp_file("SHA1LongMsg.rsp", &cal_sha1_hash, SHA1_HASH_SIZE) != 0) {
    fprintf(stderr, "FAILED!!!\n");
    ++retval;
  }

  if (retval != 0) {
    fprintf(stderr, "===== %d test(s) FAILED!!! =====\n", retval);
  } else {
    printf("All tests passed\n");
  }
  return retval;
}

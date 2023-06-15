#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mycrypto/base32.h"
#include "mycrypto/base64.h"

void print_usage(char *binary_name) {
  printf("Usage: %s [OPTION]\n\n", binary_name);
  printf("Description:\n");
  printf("  This program tests Base32/Base58/Base64 encoding/decoding\n\n");
  printf("Options:\n");
  printf("  -p, --test-case-path <path>    Path to test case file\n");
  printf("  -e, --encoding-scheme <32|64>  Encoding scheme\n");
  printf("  -h, --help                     Print this help message\n");
}

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @param out_test_case_path Caller needs to free() this char pointer
 * @param out_encoding_scheme
 */
void parse_options(int argc, char *argv[], char **out_test_case_path,
                   int *out_encoding_scheme) {
  static struct option long_options[] = {
      {"test-case-path", required_argument, 0, 'p'},
      {"encoding-scheme", required_argument, 0, 'e'},
      {"help", optional_argument, 0, 'h'},
      {0, 0, 0, 0}};

  int option_index = 0, opt;

  while ((opt = getopt_long(argc, argv, "p:e:h", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 'p':
      if (optarg != NULL) {
        *out_test_case_path = strdup(optarg);
        break;
      }
    case 'e':
      if (strcmp(optarg, "32") == 0) {
        *out_encoding_scheme = 32;
      } else if (strcmp(optarg, "64") == 0) {
        *out_encoding_scheme = 64;
      }
      break;
    default:
      print_usage(argv[0]);
    }
  }
}

int main(int argc, char *argv[]) {
  int retval = 0;
  FILE *fp;
  uint8_t input_bytes[8192];
  size_t input_len;
  char *test_case_path = NULL;
  int encoding_scheme = -1;
  parse_options(argc, argv, &test_case_path, &encoding_scheme);
  if ((encoding_scheme != 32 && encoding_scheme != 64) ||
      test_case_path == NULL) {
    print_usage(argv[0]);
    retval = 1;
    goto err_invalid_argc;
  }

  fp = fopen(test_case_path, "r");
  // No, I can't free() test_case_path here because fopen() takes a const char*
  if (fp == NULL) {
    fprintf(stdout, "Error opening file %s: %d(%s)\n", test_case_path, errno,
            strerror(errno));
    retval = 1;
    goto err_fopen;
  }

  input_len = fread(input_bytes, 1, sizeof(input_bytes), fp);
  if (input_len == 0) {
    fprintf(stderr, "Error fread()ing data from %s\n", test_case_path);
    retval = 1;
    goto err_fread;
  }

  char *output;
  if (encoding_scheme == 32) {
    output = encode_bytes_to_base32_string(input_bytes, input_len);
  } else if (encoding_scheme == 64) {
    output = encode_bytes_to_base64_string(input_bytes, input_len, 0);
  } else {
    fprintf(stderr, "How come?\n");
    abort();
  }

  if (output == NULL) {
    fprintf(stderr, "Error encoding %s\n", input_bytes);
    abort();
  } else {
    ssize_t output_len;
    uint8_t *decoded;
    if (encoding_scheme == 32) {
      decoded = decode_base32_string_to_bytes(output, &output_len);
    } else if (encoding_scheme == 64) {
      decoded = decode_base64_string_to_bytes(output, &output_len);
    } else {
      fprintf(stderr, "How come?\n");
      abort();
    }
    if (output_len >= 0) {
      // Does passing NULL pointers to memcmp() cause undefined behavior?
      // Seems it is not as clear as one might think:
      // https://stackoverflow.com/questions/16362925/can-i-pass-a-null-pointer-to-memcmp
      // But we tested output_len for non-negative value, so it should be fine.
      if (memcmp(decoded, input_bytes, output_len) != 0) {
        fprintf(stderr, "Error decoding: %s vs %s\n", (char *)decoded,
                input_bytes);
        abort();
      } else {
        printf("OK\n");
      }
    } else {
      fprintf(stderr, "Error decoding: %s\n", output);
      abort();
    }
  }

  free(output);
err_fread:
  fclose(fp);
err_fopen:
err_invalid_argc:
  free(test_case_path); // free() a NULL pointer is well-defined
  return retval;
}

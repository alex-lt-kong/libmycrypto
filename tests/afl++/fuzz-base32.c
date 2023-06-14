#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mycrypto/base32.h"

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
  char plain[4096], encoded[4096];
  char *test_case_path = NULL;
  int encoding_scheme = -1;
  parse_options(argc, argv, &test_case_path, &encoding_scheme);
  if (encoding_scheme == -1 || test_case_path == NULL) {
    print_usage(argv[0]);
    retval = 1;
    goto err_invalid_argc;
  }

  fp = fopen(test_case_path, "r");
  // No, I can't free() test_case_path here because fopen() takes a const char*
  if (fp == NULL) {
    fprintf(stdout, "Error opening file %s\n", argv[1]);
    retval = 1;
    goto err_fopen;
  }

  if (fgets(plain, sizeof(plain), fp) == NULL ||
      fgets(encoded, sizeof(encoded), fp) == NULL) {
    fprintf(stderr, "Error reading lines from %s\n", argv[1]);
    retval = 1;
    goto err_fgets;
  }
  if (strlen(plain) > 0) {
    plain[strlen(plain) - 1] = '\0';
  }
  if (strlen(encoded) > 0) {
    encoded[strlen(encoded) - 1] = '\0';
  }
  printf("%s\n%s\n", plain, encoded);

  char *output = encode_bytes_to_base32_string((uint8_t *)plain, strlen(plain));
  if (output == NULL) {
    if (strlen(plain) > 0) {
      fprintf(stderr, "Error encoding %s\n", plain);
      retval = 1;
      goto err_encode_bytes_to_base32_string;
    }
  } else {
    ssize_t output_len;
    uint8_t *decoded_output =
        decode_base32_string_to_bytes(output, &output_len);
    if (output_len >= 0) {
      if (strncmp((char *)decoded_output, plain, output_len) != 0) {
        fprintf(stderr, "Error decoding: %s vs %s\n", (char *)decoded_output,
                plain);
        retval = 1;
        abort();
      } else {
        printf("OK\n");
      }
    } else {
      fprintf(stderr, "Error!\n");
    }
  }

  free(output);
err_encode_bytes_to_base32_string:
err_fgets:
  fclose(fp);
err_fopen:
err_invalid_argc:
  free(test_case_path); // free() a NULL pointer is well-defined
  return retval;
}

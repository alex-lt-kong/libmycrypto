#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mycrypto/base32.h"

int main(int argc, char *argv[]) {
  int retval = 0;
  FILE *fp;
  char plain[4096], encoded[4096];

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
    retval = 1;
    goto err_invalid_argc;
  }
  fp = fopen(argv[1], "r");
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
  printf("%s\n", output);
err_fgets:
  fclose(fp);
err_fopen:
err_invalid_argc:
  return retval;
}
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mycrypto/base64.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: base64-encode <file_path> <chars_per_line>\n");
    return 1;
  }
  size_t chars_per_line = strtol(argv[2], NULL, 10);
  if (chars_per_line % 4 != 0) {
    fprintf(stderr, "Given how the underlying functions are designed, "
                    "chars_per_line has to be a multiple of 4\n");
    return 1;
  }

  FILE *fp = NULL;
  const size_t buf_size =
      chars_per_line == 0 ? 3 * 1024 : 1024 * chars_per_line / 4 * 3;
  size_t file_len;
  size_t retval = 0;
  size_t bytes_read = 0;
  char *b64_str = NULL;
  unsigned char *buffer = NULL;

  buffer = malloc(buf_size);
  if (buffer == NULL) {
    fprintf(stderr, "malloc() failed\n");
    retval = 1;
    goto finally;
  }

  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    fprintf(stderr, "fopen() failed\n");
    retval = 1;
    goto finally;
  }
  fseek(fp, 0, SEEK_END); // Jump to the end of the file
  file_len = ftell(fp);   // Get the current byte offset in the file
  rewind(fp);
  fprintf(stderr, "%lu bytes will be read into memory...\n", file_len);

  while (bytes_read < file_len) {
    size_t expected_bytes =
        (file_len - bytes_read) > buf_size ? buf_size : (file_len - bytes_read);

    size_t actual_bytes = fread(buffer, 1, expected_bytes, fp);
    bytes_read += actual_bytes;
    if (actual_bytes != expected_bytes) {
      fprintf(stderr,
              "fread() failed to read expected number of bytes into memory "
              "(%lu vs %lu)\n",
              actual_bytes, expected_bytes);
      retval = 1;
      goto finally;
    }
    b64_str =
        encode_bytes_to_base64_string(buffer, actual_bytes, chars_per_line);
    if (b64_str == NULL) {
      fprintf(stderr, "encode_bytes_to_base64_string() failed\n");
      retval = 1;
      goto finally;
    }
    if (strlen(b64_str) > 0 && b64_str[strlen(b64_str) - 1] == '\n') {
      //    b64_str[strlen(b64_str)-1] = '\0';
    }
    printf("%s", b64_str);
    free(b64_str);
  }
finally:

  if (buffer != NULL) {
    free(buffer);
  }
  if (fp != NULL) {
    fclose(fp);
  }
  return retval;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "../src/base64.h"

#define BUF_SIZE 57 // it has to be a multiple of 57 to make the program work. This is related to CHARS_PER_LINE

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: base64-encode <file_path>\n");
        return 1;
    }
    FILE *fp = NULL;
    uint8_t buffer[BUF_SIZE + 1];
    size_t file_len;
    size_t retval = 0;
    size_t bytes_read = 0;
    char* b64_str = NULL;

    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed\n");
        retval = 1;
        goto finally;
    }
    fseek(fp, 0, SEEK_END);          // Jump to the end of the file
    file_len = ftell(fp);             // Get the current byte offset in the file
    rewind(fp); 
    fprintf(stderr, "%lu bytes will be read into memory...\n", file_len);

    while (bytes_read < file_len) {
        size_t expected_bytes = (file_len - bytes_read) > BUF_SIZE ? BUF_SIZE : (file_len - bytes_read);
        
        size_t actual_bytes = fread(buffer, 1, expected_bytes, fp);
        bytes_read += actual_bytes;
        if (actual_bytes != expected_bytes) {
            fprintf(
                stderr,
                "fread() failed to read expected number of bytes into memory (%lu vs %lu)\n",
                actual_bytes, expected_bytes
            );
            retval = 1;
            goto finally;
        }
        b64_str = encode_bytes_to_base64_string(buffer, actual_bytes, false);
        if (b64_str == NULL) {
            fprintf(stderr, "encode_bytes_to_base64_string() failed\n");        
            retval = 1;
            goto finally;        
        }
        if (strlen(b64_str) > 0 && b64_str[strlen(b64_str)-1] == '\n') {
            b64_str[strlen(b64_str)-1] = '\0';
        }
        printf("%s", b64_str);
        free(b64_str);
    }
finally:

    if (fp != NULL) {
        fclose(fp);
    }
    return retval;
}
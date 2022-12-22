#include <stdio.h>
#include <stdlib.h>

#include "../src/base64.h"


int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: base64-encode <file_path>\n");
        return 1;
    }
    FILE *fp = NULL;
    uint8_t *buffer;
    long file_len;
    size_t retval = 0;
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
    buffer = malloc(file_len * sizeof(uint8_t));
    if (buffer == NULL) {
        fprintf(stderr, "malloc() failed\n");
        retval = 1;
        goto finally;
    }
    fprintf(stderr, "reading %lu bytes into memory...\n", file_len);
    fread(buffer, file_len, 1, fp); // Read in the entire file
    b64_str = encode_bytes_to_base64_string(buffer, file_len);
    if (b64_str == NULL) {
        fprintf(stderr, "encode_bytes_to_base64_string() failed, most likely because of malloc() failure\n");        
        retval = 1;
        
    }
    printf("%s\n", b64_str);
finally:
    if (b64_str != NULL) {
        free(b64_str);
    }
    if (fp != NULL) {
        fclose(fp);
    }
    return retval;
}
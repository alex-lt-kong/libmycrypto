#include <stdio.h>
#include <stdlib.h>

#include "../src/base64.h"


int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: base64-encode <file_path>\n");
        return 1;
    }
    FILE *fp;
    uint8_t *buffer;
    long file_len;

    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "fopen() failed\n");
        return 1;
    }
    fseek(fp, 0, SEEK_END);          // Jump to the end of the file
    file_len = ftell(fp);             // Get the current byte offset in the file
    rewind(fp); 
    buffer = malloc(file_len * sizeof(uint8_t));
    if (buffer == NULL) {
        fprintf(stderr, "malloc() failed\n");
        fclose(fp);
        return 1;
    }
    printf("reading %u bytes into memory...\n", file_len);
    fread(buffer, file_len, 1, fp); // Read in the entire file
    char* b64_str = encode_bytes_to_base64_string(buffer, file_len);
    if (b64_str == NULL) {
        fprintf(stderr, "malloc() failed\n");
        fclose(fp);
        return 1;
    }
    printf("%s\n", b64_str);
    free(b64_str);
    fclose(fp); // Close the file
    return 0;
}
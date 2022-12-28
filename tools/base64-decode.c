#include <stdio.h>
#include <stdlib.h>

#include "libmycrypto/base64.h"


int main (int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: <src filepath> <dest filepath>\n");
        return 1;
    }
    FILE* fp_in = NULL;
    FILE *fp_out = NULL;
    uint8_t* buffer = NULL;
    uint8_t* bytes_out = NULL;
    size_t file_len;
    size_t retval = 0;
    fp_in = fopen(argv[1], "rb");          // Open the file in binary mode
    if (fp_in == NULL) {
        fprintf(stderr, "fopen() failed\n");
        retval = 1;
        goto finally;
    }
    fseek(fp_in, 0, SEEK_END);          // Jump to the end of the file
    file_len = ftell(fp_in);             // Get the current byte offset in the file
    rewind(fp_in); 
    buffer = malloc(file_len * sizeof(uint8_t));
 
    if (NULL == buffer) {
        fprintf(stderr, "malloc() failed\n");
        retval = 1;
        goto finally;
    }
    fread(buffer, file_len, 1, fp_in);
    int64_t output_len;
    
    bytes_out = decode_base64_string_to_bytes((char*)buffer, &output_len);
    if (NULL == bytes_out) {
        fprintf(stderr, "decode_base64_string_to_bytes() failed, most likely due to malloc() failure\n");
        retval = 1;
        goto finally;
    }
    

    fp_out = fopen(argv[2], "wb");
    if (fp_out == NULL) {
        fprintf(stderr, "fopen() failed\n");
        retval = 1;
        goto finally;
    }
    size_t bytes_written = fwrite(bytes_out, sizeof(uint8_t), output_len, fp_out);
    if (bytes_written < (size_t)output_len) {
        fprintf(stderr, "fwrite() failed, expecting %lu bytes but write %lu bytes only\n", output_len, bytes_written);
        retval = 1;
        goto finally;
    }
finally:
    if (fp_out != NULL) {
        fclose(fp_out);
    }
    if (fp_in != NULL) {
        fclose(fp_in);
    }
    if (buffer != NULL) {
        free(buffer);
    }
    if (bytes_out != NULL) {
        free(bytes_out);
    }
    return retval;
}

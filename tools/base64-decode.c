#include <stdio.h>
#include <stdlib.h>

#include "../src/base64.h"


int main (int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: <src filepath> <dest filepath>\n");
        return 1;
    }
    FILE* fp_in;
    uint8_t* buffer;
    size_t file_len;

    fp_in = fopen(argv[1], "rb");          // Open the file in binary mode
    if (fp_in == NULL) {
        fprintf(stderr, "fopen() failed\n");
        return 1;
    }
    fseek(fp_in, 0, SEEK_END);          // Jump to the end of the file
    file_len = ftell(fp_in);             // Get the current byte offset in the file
    rewind(fp_in); 
    buffer = malloc(file_len * sizeof(uint8_t));
 
    if (NULL == buffer) {
        fprintf(stderr, "malloc() failed\n");
        fclose(fp_in);
        return 1;
    }
    fread(buffer, file_len, 1, fp_in);
    size_t output_len;
    uint8_t* bytes_out;
    bytes_out = decode_base64_string_to_bytes(buffer, &output_len);
    if (NULL == bytes_out) {
        fprintf(stderr, "malloc() failed\n");
        fclose(fp_in);
        free(buffer);
        return 1;
    }
    FILE *fp_out;

    fp_out = fopen(argv[2],"wb");  // w for write, b for binary
    if (fp_out == NULL) {
        fprintf(stderr, "fopen() failed\n");
        fclose(fp_in);
        free(buffer);
        free(bytes_out);
        return 1;
    }
    fwrite(bytes_out, output_len, 1, fp_out); // write 10 bytes from our buffer
    fclose(fp_out);
    fclose(fp_in);
    free(buffer);
    free(bytes_out);
    return 0;
}
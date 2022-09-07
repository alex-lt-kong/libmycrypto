#include <stdio.h>
#include <stdlib.h>

#include "../src/base64.h"

#define BUFF_SIZE 10000000

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: <file to read>");
    }
    FILE* ptr;
    uint8_t* buf = malloc(BUFF_SIZE);
 
    // Opening file in reading mode
    ptr = fopen(argv[1], "r");
 
    if (NULL == ptr) {
        fprintf(stderr, "File [%s] can't be opened!\n", argv[1]);
        return 1;
    }
    int count = fread(buf, sizeof(uint8_t), BUFF_SIZE, ptr);
    if (count == BUFF_SIZE) {
        printf("WARNING: buffer is full, some content may be left unread!\n");
    }
    size_t output_len;
    uint8_t* out;
    out = decode_base64_string_to_bytes(buf, &output_len);
    printf("%s\n", out);
    fclose(ptr);
    free(buf);
    free(out);
    return 0;
}
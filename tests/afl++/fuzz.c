#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mycrypto/base32.h"
#include "mycrypto/base58.h"
#include "mycrypto/base64.h"
#include "mycrypto/hmac.h"
#include "mycrypto/misc.h"
#include "mycrypto/ripemd160.h"
#include "mycrypto/sha1.h"
#include "mycrypto/sha256.h"

#define PRINTF(fmt, ...)                                                       \
  printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define FPRINTF_ERR(fmt, ...)                                                  \
  fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define MY_BUF_SIZE 65536

const char hmac_key[] = "Hello world!";

void print_usage(char *binary_name) {
  printf("Usage: %s [OPTION]\n\n", binary_name);
  printf("Description:\n");
  printf("  This program tests "
         "base32/base58/base64/sha1/sha256/ripemd160/hmac-sha1 "
         "encoding/decoding schemes\n\n");
  printf("Options:\n");
  printf("  -p, --test-case-path <path>   Path to test case file\n");
  printf("  -s, --scheme <Scheme>         Scheme\n");
  printf("  -h, --help                    Print this help message\n");
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
                   char **out_scheme) {
  static struct option long_options[] = {
      {"test-case-path", required_argument, 0, 'p'},
      {"scheme", required_argument, 0, 's'},
      {"help", optional_argument, 0, 'h'},
      {0, 0, 0, 0}};

  int option_index = 0, opt;

  while ((opt = getopt_long(argc, argv, "p:s:h", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 'p':
      if (optarg != NULL) {
        *out_test_case_path = strdup(optarg);
      }
      break;
    case 's':
      if (strcmp(optarg, "base32") == 0 || strcmp(optarg, "base58") == 0 ||
          strcmp(optarg, "base64") == 0 || strcmp(optarg, "sha256") == 0 ||
          strcmp(optarg, "sha1") == 0 || strcmp(optarg, "ripemd160") == 0 ||
          strcmp(optarg, "hmac-sha1") == 0) {
        *out_scheme = strdup(optarg);
      }
      break;
    default:
      print_usage(argv[0]);
      exit(0);
    }
  }
}

void compare_with_external_results(const char *scheme,
                                   const char *test_case_path,
                                   const char *actual_output) {

  int pipefd_out[2], pipefd_err[2];
  FILE *fp_out;
  FILE *fp_err;
  int status;

  if (pipe(pipefd_out) == -1 || pipe(pipefd_err) == -1) {
    perror("pipe()");
    abort();
  }

  pid_t child_pid = fork();
  if (child_pid == -1) { // fork() failed, no child process created
    perror("fork()");
    abort();
  }

  if (child_pid == 0) { // fork() succeeded, we are in the child process
    close(pipefd_out[0]);
    close(pipefd_err[0]);
    dup2(pipefd_out[1], STDOUT_FILENO);
    dup2(pipefd_err[1], STDERR_FILENO);

    const char *args[8];
    if (strcmp(scheme, "base32") == 0) {
      args[0] = "/usr/bin/base32";
      args[1] = test_case_path;
      args[2] = "-w";
      args[3] = "0";
      args[4] = NULL;
    } else if (strcmp(scheme, "base58") == 0) {
      args[0] = "/usr/bin/base58";
      args[1] = test_case_path;
      args[2] = NULL;
      execv(args[0], (char **)args);
    } else if (strcmp(scheme, "base64") == 0) {
      args[0] = "/usr/bin/base64";
      args[1] = test_case_path;
      args[2] = "-w";
      args[3] = "0";
      args[4] = NULL;
      execv(args[0], (char **)args);
    } else if (strcmp(scheme, "sha1") == 0) {
      args[0] = "/usr/bin/sha1sum";
      args[1] = test_case_path;
      args[2] = NULL;
      execv(args[0], (char **)args);
    } else if (strcmp(scheme, "sha256") == 0) {
      args[0] = "/usr/bin/sha256";
      args[1] = test_case_path;
      args[2] = NULL;
    } else if (strcmp(scheme, "ripemd160") == 0) {
      args[0] = "/usr/bin/rhash";
      args[1] = "--ripemd160";
      args[2] = test_case_path;
      args[3] = NULL;
    } else if (strcmp(scheme, "hmac-sha1") == 0) {
      args[0] = "/usr/bin/openssl";
      args[1] = "dgst";
      args[2] = "-sha1";
      args[3] = "-hmac";
      args[4] = hmac_key;
      args[5] = test_case_path;
      args[6] = NULL;
    } else {
      FPRINTF_ERR(
          "compare_with_external_results() get an unexpected parameter: %s",
          scheme);
      abort();
    }
    execv(args[0], (char **)args);

    perror("execl()/execv()");
    // The exec() functions return only if an error has occurred.
    // The return value is -1, and errno is set to indicate the error.
    _exit(EXIT_FAILURE);
    /* Have to _exit() explicitly in case of execl() failure.
       Difference between _exit() and exit()? refer to:
       https://github.com/alex-lt-kong/the-nitty-gritty/blob/main/c-cpp/common/04_posix-api/01_subprocess/main-naive.c
    */
  }

  // Only parent gets here
  close(pipefd_out[1]);
  close(pipefd_err[1]);

  if ((fp_out = fdopen(pipefd_out[0], "r")) == NULL ||
      (fp_err = fdopen(pipefd_err[0], "r")) == NULL) {
    perror("fdopen()");
    abort();
  }

  char buff[MY_BUF_SIZE];
  char output[MY_BUF_SIZE] = {0};
  size_t output_len = 0;
  while (fgets(buff, MY_BUF_SIZE, fp_out)) {
    size_t buffer_len = strlen(buff);

    if (output_len + buffer_len + 1 > MY_BUF_SIZE) {
      FPRINTF_ERR("output too long!\n");
      abort();
    }
    memcpy(output + output_len, buff, buffer_len);
    output_len += buffer_len;
  }

  if (fclose(fp_out) != 0 || fclose(fp_err) != 0) {
    perror("fclose()");
    abort();
  }
  close(pipefd_out[0]);
  close(pipefd_err[0]);

  // wait for the child process to terminate
  if (waitpid(child_pid, &status, 0) == -1) {
    perror("waitpid()");
    abort();
  }

  if (WIFEXITED(status)) {
    PRINTF("Child process exited normally, rc: %d\n", WEXITSTATUS(status));
  } else {
    PRINTF("Child process exited unexpectedly ");
    if (WIFSIGNALED(status)) {
      PRINTF("(terminated by a signal: %d)\n", WTERMSIG(status));
      abort();
    } else if (WIFSTOPPED(status)) {
      PRINTF("(stopped by delivery of a signal: %d)\n", WSTOPSIG(status));
      abort();
    } else {
      PRINTF("(unknown status: %d)\n", status);
      abort();
    }
  }
  if (strcmp(scheme, "sha256") == 0 &&
      strncmp(buff, actual_output, SHA256_HASH_SIZE) == 0) {
    PRINTF("OK!\n");
  } else if (strcmp(scheme, "sha1") == 0 &&
             strncmp(buff, actual_output, SHA1_HASH_SIZE) == 0) {
    PRINTF("OK!\n");
  } else if (strcmp(scheme, "ripemd160") == 0 &&
             strncmp(buff, actual_output, RIPEMD160_HASH_SIZE) == 0) {
    PRINTF("OK!\n");
  } else if (strcmp(scheme, "hmac-sha1") == 0 &&
             strncmp(buff + strlen(buff) - SHA1_HASH_SIZE * 2 - 1,
                     actual_output, SHA1_HASH_SIZE) == 0) {

    PRINTF("OK!\n");
  } else if (strcmp(buff, actual_output) == 0) {
    PRINTF("OK!\n");
  } else {
    FPRINTF_ERR("\nExpect: %s\nActual: %s\n", buff, actual_output);
  }
}

int main(int argc, char *argv[]) {
  int retval = 0;
  FILE *fp;
  unsigned char input_bytes[MY_BUF_SIZE];
  size_t input_len;
  char *test_case_path = NULL;
  char *scheme = NULL;
  parse_options(argc, argv, &test_case_path, &scheme);
  if (scheme == NULL ||
      (strcmp(scheme, "base32") != 0 && strcmp(scheme, "base58") != 0 &&
       strcmp(scheme, "base64") != 0 && strcmp(scheme, "sha1") != 0 &&
       strcmp(scheme, "sha256") != 0 && strcmp(scheme, "ripemd160") != 0 &&
       strcmp(scheme, "hmac-sha1") != 0) ||
      test_case_path == NULL) {
    print_usage(argv[0]);
    retval = -1;
    goto err_invalid_argc;
  }

  fp = fopen(test_case_path, "r");
  // No, I can't free() test_case_path here because fopen() takes a const char*
  if (fp == NULL) {
    FPRINTF_ERR("Error opening file %s: %d(%s)\n", test_case_path, errno,
                strerror(errno));
    retval = 1;
    goto err_fopen;
  }

  input_len = fread(input_bytes, 1, sizeof(input_bytes), fp);
  if (input_len == 0) {
    FPRINTF_ERR("Error fread()ing data from %s\n", test_case_path);
    retval = 1;
    goto err_fread;
  }

  char *output = NULL;
  if (strcmp(scheme, "base32") == 0) {
    output = encode_bytes_to_base32_string(input_bytes, input_len);
  } else if (strcmp(scheme, "base58") == 0) {
    output = encode_bytes_to_base58_string(input_bytes, input_len);
  } else if (strcmp(scheme, "base64") == 0) {
    output = encode_bytes_to_base64_string(input_bytes, input_len, 0);
  } else if (strcmp(scheme, "sha1") == 0) {
    unsigned char hash_val[SHA1_HASH_SIZE] = {0};
    if (cal_sha1_hash(input_bytes, input_len, hash_val) != 0) {
      FPRINTF_ERR("cal_sha1_hash() failed");
      abort();
    }
    output = bytes_to_hex_string(hash_val, SHA1_HASH_SIZE, false);
    if (output == NULL) {
      fprintf(stderr, "bytes_to_hex_string() failed\n");
      abort();
    }
  } else if (strcmp(scheme, "sha256") == 0) {
    unsigned char hash_val[SHA256_HASH_SIZE] = {0};
    if (cal_sha256_hash(input_bytes, input_len, hash_val) != 0) {
      FPRINTF_ERR("cal_sha256_hash() failed");
      abort();
    }
    output = bytes_to_hex_string(hash_val, SHA256_HASH_SIZE, false);
    if (output == NULL) {
      fprintf(stderr, "bytes_to_hex_string() failed\n");
      abort();
    }
  } else if (strcmp(scheme, "ripemd160") == 0) {
    unsigned char hash_val[RIPEMD160_HASH_SIZE] = {0};
    cal_ripemd160_hash(input_bytes, input_len, hash_val);
    output = bytes_to_hex_string(hash_val, RIPEMD160_HASH_SIZE, false);
    if (output == NULL) {
      fprintf(stderr, "bytes_to_hex_string() failed\n");
      abort();
    }
  } else if (strcmp(scheme, "hmac-sha1") == 0) {
    unsigned char hash_val[SHA1_HASH_SIZE] = {0};
    hmac_sha1((const unsigned char *)hmac_key, strlen(hmac_key), input_bytes,
              input_len, hash_val);
    output = bytes_to_hex_string(hash_val, SHA1_HASH_SIZE, false);
    if (output == NULL) {
      fprintf(stderr, "bytes_to_hex_string() failed\n");
      abort();
    }
  } else {
    FPRINTF_ERR("main() received unexpected parameter: %s\n", scheme);
    abort();
  }

  if (output == NULL) {
    fprintf(stderr, "Error encoding %s\n", input_bytes);
    abort();
  } else {
    ssize_t output_len = 0;
    unsigned char *decoded = NULL;
    if (strcmp(scheme, "base32") == 0) {
      decoded = decode_base32_string_to_bytes(output, &output_len);
    } else if (strcmp(scheme, "base64") == 0) {
      decoded = decode_base64_string_to_bytes(output, &output_len);
    } else if (strcmp(scheme, "base58") == 0 || strcmp(scheme, "sha1") == 0 ||
               strcmp(scheme, "sha256") == 0 ||
               strcmp(scheme, "ripemd160") == 0 ||
               strcmp(scheme, "hmac-sha1") == 0) {
      PRINTF("%s doesn't have decoding method, skipped\n", scheme);
    } else {
      FPRINTF_ERR("Output comparison received an unexpected parameter: %s\n",
                  scheme);
      abort();
    }
    if (output_len >= 0 && strcmp(scheme, "base58") != 0 &&
        strcmp(scheme, "sha1") != 0 && strcmp(scheme, "sha256") != 0 &&
        strcmp(scheme, "ripemd160") != 0 && strcmp(scheme, "hmac-sha1") != 0) {
      // Does passing NULL pointers to memcmp() cause undefined behavior?
      // Seems it is not as clear as one might think:
      // https://stackoverflow.com/questions/16362925/can-i-pass-a-null-pointer-to-memcmp
      // But we tested output_len for non-negative value, so it should be fine.
      if (memcmp(decoded, input_bytes, output_len) != 0) {
        FPRINTF_ERR("Error decoding: %s vs %s\n", (char *)decoded, input_bytes);
        abort();
      } else {
        PRINTF("OK\n");
      }
    } else if (strcmp(scheme, "base58") == 0 || strcmp(scheme, "sha1") == 0 ||
               strcmp(scheme, "sha256") == 0 ||
               strcmp(scheme, "ripemd160") == 0 ||
               strcmp(scheme, "hmac-sha1") == 0) {
    } else {
      FPRINTF_ERR("Error decoding: %s\n", output);
      abort();
    }
  }
  compare_with_external_results(scheme, test_case_path, output);
  free(output);
err_fread:
  fclose(fp);
err_fopen:
err_invalid_argc:
  free(test_case_path); // free() a NULL pointer is well-defined
  free(scheme);
  return retval;
}

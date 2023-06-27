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

#define PRINTF(fmt, ...)                                                       \
  printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define FPRINTF_ERR(fmt, ...)                                                  \
  fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define MY_BUF_SIZE 32768

void print_usage(char *binary_name) {
  printf("Usage: %s [OPTION]\n\n", binary_name);
  printf("Description:\n");
  printf("  This program tests Base32/Base58/Base64 encoding/decoding\n\n");
  printf("Options:\n");
  printf("  -p, --test-case-path <path>         Path to test case file\n");
  printf("  -s, --scheme <base32|base58|base64> Scheme\n");
  printf("  -h, --help                          Print this help message\n");
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
          strcmp(optarg, "base64") == 0) {
        *out_scheme = strdup(optarg);
      }
      break;
    default:
      print_usage(argv[0]);
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

    if (strcmp(scheme, "base32") == 0) {
      const char *args[] = {"/usr/bin/base32", test_case_path, "-w", "0", NULL};
      execv(args[0], (char **)args);
    } else if (strcmp(scheme, "base58") == 0) {
      const char *args[] = {"/usr/bin/base58", test_case_path, NULL};
      execv(args[0], (char **)args);
    } else if (strcmp(scheme, "base642") == 0) {
      const char *args[] = {"/usr/bin/base64", test_case_path, "-w", "0", NULL};
      execv(args[0], (char **)args);
    }

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
  if (strcmp(buff, actual_output) == 0) {
    PRINTF("OK!\n");
  } else {
    FPRINTF_ERR("\nExpect: %s\nActual: %s\n", buff, actual_output);
  }
}

int main(int argc, char *argv[]) {
  int retval = 0;
  FILE *fp;
  uint8_t input_bytes[MY_BUF_SIZE];
  size_t input_len;
  char *test_case_path = NULL;
  char *scheme = NULL;
  parse_options(argc, argv, &test_case_path, &scheme);
  if (scheme == NULL ||
      (strcmp(scheme, "base32") != 0 && strcmp(scheme, "base58") != 0 &&
       strcmp(scheme, "base64") != 0) ||
      test_case_path == NULL) {
    print_usage(argv[0]);
    retval = 1;
    goto err_invalid_argc;
  }

  fp = fopen(test_case_path, "r");
  // No, I can't free() test_case_path here because fopen() takes a const char*
  if (fp == NULL) {
    fprintf(stdout, "Error opening file %s: %d(%s)\n", test_case_path, errno,
            strerror(errno));
    retval = 1;
    goto err_fopen;
  }

  input_len = fread(input_bytes, 1, sizeof(input_bytes), fp);
  if (input_len == 0) {
    fprintf(stderr, "Error fread()ing data from %s\n", test_case_path);
    retval = 1;
    goto err_fread;
  }

  char *output;
  if (strcmp(scheme, "base32") == 0) {
    output = encode_bytes_to_base32_string(input_bytes, input_len);
  } else if (strcmp(scheme, "base58") == 0) {
    output = encode_bytes_to_base58_string(input_bytes, input_len);
  } else if (strcmp(scheme, "base64") == 0) {
    output = encode_bytes_to_base64_string(input_bytes, input_len, 0);
  } else {
    fprintf(stderr, "How come?\n");
    abort();
  }

  if (output == NULL) {
    fprintf(stderr, "Error encoding %s\n", input_bytes);
    abort();
  } else {
    ssize_t output_len = 0;
    uint8_t *decoded = NULL;
    if (strcmp(scheme, "base32") == 0) {
      decoded = decode_base32_string_to_bytes(output, &output_len);
    } else if (strcmp(scheme, "base58") == 0) {
      PRINTF("Base58 doesn't have decoding method, skipped\n");
    } else if (strcmp(scheme, "base64") == 0) {
      decoded = decode_base64_string_to_bytes(output, &output_len);
    } else {
      fprintf(stderr, "How come?\n");
      abort();
    }
    if (output_len >= 0 && strcmp(scheme, "base58") != 0) {
      // Does passing NULL pointers to memcmp() cause undefined behavior?
      // Seems it is not as clear as one might think:
      // https://stackoverflow.com/questions/16362925/can-i-pass-a-null-pointer-to-memcmp
      // But we tested output_len for non-negative value, so it should be fine.
      if (memcmp(decoded, input_bytes, output_len) != 0) {
        fprintf(stderr, "Error decoding: %s vs %s\n", (char *)decoded,
                input_bytes);
        abort();
      } else {
        PRINTF("OK\n");
      }
    } else if (strcmp(scheme, "base58") == 0) {
    } else {
      fprintf(stderr, "Error decoding: %s\n", output);
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

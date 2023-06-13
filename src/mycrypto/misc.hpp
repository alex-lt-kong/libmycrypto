#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <cstdlib>
#include <memory>

#include "misc.h"

// According to this link:
// https://stackoverflow.com/questions/76453866/stdunique-ptr-with-custom-deleter-for-wrapping-a-malloc-pointer
// The below design should be good enough

struct FreeDeleter {
  void operator()(void *p) const {
    // std::free() does nothing if p is NULL
    std::free(p);
  }
};

template <typename T> using unique_fptr = std::unique_ptr<T, FreeDeleter>;

#endif

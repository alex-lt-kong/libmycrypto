#ifndef LMC_MISC_HPP
#define LMC_MISC_HPP

#include "misc.h"

#include <cstdlib>
#include <memory>

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

#endif /* LMC_MISC_HPP */

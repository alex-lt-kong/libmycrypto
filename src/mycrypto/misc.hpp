#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <cstdlib>
#include <memory>

#include "misc.h"

struct FreeDeleter
{
    void operator()(void *p) const
    {
        std::free(p);
    }
};

template <typename T>
using unique_fptr = std::unique_ptr<T, FreeDeleter>;


#endif

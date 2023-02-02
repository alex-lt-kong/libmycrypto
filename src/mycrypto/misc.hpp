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

using unique_byte_ptr = std::unique_ptr<uint8_t, FreeDeleter>;
using unique_char_ptr = std::unique_ptr<char, FreeDeleter>;

#endif

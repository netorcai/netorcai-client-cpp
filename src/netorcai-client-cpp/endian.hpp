#pragma once

// This is a basic implementation of std::endian.
// Prefer including <type_traits> if you can use a recent C++ compiler.
// https://howardhinnant.github.io/endian.html

enum class endian
{
#ifdef _WIN32
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

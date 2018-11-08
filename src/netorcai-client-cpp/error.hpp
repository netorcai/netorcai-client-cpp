#pragma once

#include <stdarg.h>
#include <stdio.h>

namespace netorcai
{

struct Error : std::exception
{
    char text[1024];

    Error(char const* fmt, ...) __attribute__((format(printf,2,3))) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(text, sizeof text, fmt, ap);
        va_end(ap);
    }

    char const* what() const throw() { return text; }
};

} // end of netorcai namespace

/// Define an ENFORCE macro similar to D's enforce.
#define NETORCAI_ENFORCE(pred, fmt, ...) \
    do { \
        if (false == (pred)) { \
            throw netorcai::Error("%s:%d:%s assert failed: " fmt "\naborting", \
                                __FILE__, __LINE__, __func__,  ##__VA_ARGS__); \
        } \
    } while (0)

/// Define an ASSERT macro similar to D's assert.
#ifndef NDEBUG
    #define NETORCAI_ASSERT NETORCAI_ENFORCE
#else
    #define NETORCAI_ASSERT(pred, fmt, ...) do {} while (0)
#endif

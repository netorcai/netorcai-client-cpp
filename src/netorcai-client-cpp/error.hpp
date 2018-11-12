#pragma once

#include <cstdarg>
#include <cstdio>

namespace netorcai
{

/// netorcai's exception class
struct Error : std::exception
{
    char text[1024]; //!< The error message

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /// Builds an Error with a printf-like format
    Error(char const* fmt, ...) __attribute__((format(printf,2,3)))
    {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(text, sizeof text, fmt, ap);
        va_end(ap);
    }
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

    /// Gets Error's message
    /// @return The Error's message.
    char const* what() const throw() { return text; }
};

} // end of netorcai namespace

/// Define an ENFORCE macro similar to D's enforce.
#define NETORCAI_ENFORCE(pred, fmt, ...) \
    do { \
        if (false == (pred)) { \
            throw netorcai::Error(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

/// Define an ASSERT macro similar to D's assert.
#ifndef NDEBUG
    #define NETORCAI_ASSERT NETORCAI_ENFORCE
#else
    #define NETORCAI_ASSERT(pred, fmt, ...) do {} while (0)
#endif

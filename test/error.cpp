#include "catch.hpp"

#include <netorcai-client-cpp/error.hpp>

void enforce_wrapper(bool pred)
{
    ENFORCE(pred, "");
}

void assert_wrapper(bool pred)
{
    ASSERT(pred, "");
}

#include <stdio.h>

TEST_CASE("ASSERT calls", "[error]")
{
    #ifdef NDEBUG
        CHECK_NOTHROW(assert_wrapper(true), "ASSERT(true) should never throw");
        CHECK_NOTHROW(assert_wrapper(false), "ASSERT(false) should NOT throw when NDEBUG is set");
    #else
        CHECK_NOTHROW(assert_wrapper(true), "ASSERT(true) should never throw");
        CHECK_THROWS(assert_wrapper(false), "ASSERT(false) should throw when NDEBUG is unset");
    #endif
}

TEST_CASE("ENFORCE calls", "[error]")
{
    CHECK_NOTHROW(enforce_wrapper(true), "ENFORCE(true) should never throw");
    CHECK_THROWS(enforce_wrapper(false), "ENFORCE(false) should always throw");
}

#include <gtest/gtest.h>

#include <netorcai-client-cpp/error.hpp>

void enforce_wrapper(bool pred)
{
    ENFORCE(pred, "");
}

void assert_wrapper(bool pred)
{
    ASSERT(pred, "");
}

TEST(error, assert)
{
    #ifdef NDEBUG
        EXPECT_NO_THROW(assert_wrapper(true));
        EXPECT_NO_THROW(assert_wrapper(false));
    #else
        EXPECT_NO_THROW(assert_wrapper(true));
        EXPECT_THROW(assert_wrapper(false), NetorcaiError);
    #endif
}

TEST(error, enforce)
{
    EXPECT_NO_THROW(enforce_wrapper(true));
    EXPECT_THROW(enforce_wrapper(false), NetorcaiError);
}

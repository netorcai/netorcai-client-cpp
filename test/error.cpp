#include <gtest/gtest.h>

#include <netorcai-client-cpp/error.hpp>

void enforce_wrapper(bool pred)
{
    NETORCAI_ENFORCE(pred, "enforce_wrapper");
}

void assert_wrapper(bool pred)
{
    NETORCAI_ASSERT(pred, "assert_wrapper");
}

TEST(error, assert)
{
    #ifdef NDEBUG
        EXPECT_NO_THROW(assert_wrapper(true));
        EXPECT_NO_THROW(assert_wrapper(false));
    #else
        EXPECT_NO_THROW(assert_wrapper(true));
        EXPECT_THROW(assert_wrapper(false), netorcai::Error);
        bool expected_path = false;
        try
        {
            assert_wrapper(false);
        }
        catch (const netorcai::Error & e)
        {
            std::string exception_msg = e.what();
            EXPECT_NE(exception_msg.find("assert_wrapper"), std::string::npos);
            expected_path = true;
        }
        EXPECT_TRUE(expected_path);
    #endif
}

TEST(error, enforce)
{
    EXPECT_NO_THROW(enforce_wrapper(true));
    EXPECT_THROW(enforce_wrapper(false), netorcai::Error);
    bool expected_path = false;
    try
    {
        enforce_wrapper(false);
    }
    catch (const netorcai::Error & e)
    {
        std::string exception_msg = e.what();
        EXPECT_NE(exception_msg.find("enforce_wrapper"), std::string::npos);
        expected_path = true;
    }
    EXPECT_TRUE(expected_path);
}

#include "catch.hpp"

#include <netorcai-client-cpp/message.hpp>

TEST_CASE("useless function", "[message]")
{
    CHECK_NOTHROW(useless_function(4), "");
}

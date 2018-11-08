#include <gtest/gtest.h>

#include <string>

#include <netorcai-client-cpp/message.hpp>

using namespace std;
using namespace nlohmann;

TEST(message, parsePlayerInfo)
{
    const string s = R"({
      "player_id": 0,
      "nickname": "jugador",
      "remote_address": "127.0.0.1:59840",
      "is_connected": true
    })";
    json object = json::parse(s);

    const PlayerInfo pinfo = parsePlayerInfo(object);
    EXPECT_EQ(pinfo.playerID, 0);
    EXPECT_EQ(pinfo.nickname, "jugador");
    EXPECT_EQ(pinfo.remoteAddress, "127.0.0.1:59840");
    EXPECT_EQ(pinfo.isConnected, true);
}

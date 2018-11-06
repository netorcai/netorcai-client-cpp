#include <gtest/gtest.h>

#include <string>

#include <netorcai-client-cpp/message.hpp>

using namespace std;
using namespace rapidjson;

TEST(message, parsePlayerInfo)
{
    const string s = R"({
      "player_id": 0,
      "nickname": "jugador",
      "remote_address": "127.0.0.1:59840",
      "is_connected": true
    })";
    Document d;
    d.Parse(s.c_str());

    const PlayerInfo pinfo = parsePlayerInfo(d.GetObject());
    EXPECT_EQ(pinfo.playerID, 0);
    EXPECT_EQ(pinfo.nickname, "jugador");
    EXPECT_EQ(pinfo.remoteAddress, "127.0.0.1:59840");
    EXPECT_EQ(pinfo.isConnected, true);
}

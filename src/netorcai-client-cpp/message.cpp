#include "message.hpp"
#include "error.hpp"

namespace netorcai
{

PlayerInfo parsePlayerInfo(const netorcai::json & object)
{
    PlayerInfo info;
    info.playerID = object["player_id"];
    info.nickname = object["nickname"];
    info.remoteAddress = object["remote_address"];
    info.isConnected = object["is_connected"];

    return info;
}

} // end of netorcai namespace

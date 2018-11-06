#include "message.hpp"
#include "error.hpp"

PlayerInfo parsePlayerInfo(const rapidjson::Value & json)
{
    PlayerInfo info;
    info.playerID = json["player_id"].GetInt();
    info.nickname = json["nickname"].GetString();
    info.remoteAddress = json["remote_address"].GetString();
    info.isConnected = json["is_connected"].GetBool();

    return info;
}

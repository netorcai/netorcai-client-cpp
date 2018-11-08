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

std::vector<PlayerInfo> parsePlayersInfo(const netorcai::json & array)
{
    int arraySize = array.size();

    std::vector<PlayerInfo> infos;
    infos.reserve(arraySize);

    for (int i = 0; i < arraySize; i++)
    {
        infos.push_back(parsePlayerInfo(array[i]));
    }

    return infos;
}

GameStartsMessage parseGameStartsMessage(const netorcai::json & object)
{
    GameStartsMessage m;

    m.playerID = object["player_id"];
    m.nbPlayers = object["nb_players"];
    m.nbTurnsMax = object["nb_turns_max"];
    m.msBeforeFirstTurn = object["milliseconds_before_first_turn"];
    m.msBetweenTurns = object["milliseconds_between_turns"];
    m.initialGameState = object["initial_game_state"];
    m.playersInfo = parsePlayersInfo(object["players_info"]);

    return m;
}

} // end of netorcai namespace

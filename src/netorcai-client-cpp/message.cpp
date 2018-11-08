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

    NETORCAI_ENFORCE(m.initialGameState.is_object(), "initial_game_state is not an object");

    return m;
}

GameEndsMessage parseGameEndsMessage(const json & object)
{
    GameEndsMessage m;
    m.winnerPlayerID = object["winner_player_id"];
    m.gameState = object["game_state"];

    NETORCAI_ENFORCE(m.gameState.is_object(), "game_state is not an object");

    return m;
}

TurnMessage parseTurnMessage(const json & object)
{
    TurnMessage m;
    m.turnNumber = object["turn_number"];
    m.playersInfo = parsePlayersInfo(object["players_info"]);
    m.gameState = object["game_state"];

    NETORCAI_ENFORCE(m.gameState.is_object(), "game_state is not an object");

    return m;
}

DoInitMessage parseDoInitMessage(const json & object)
{
    DoInitMessage m;
    m.nbPlayers = object["nb_players"];
    m.nbTurnsMax = object["nb_turns_max"];

    return m;
}

PlayerActions parsePlayerActions(const json & object)
{
    PlayerActions pa;
    pa.playerID = object["player_id"];
    pa.turnNumber = object["turn_number"];
    pa.actions = object["actions"];

    NETORCAI_ENFORCE(pa.actions.is_array(), "actions is not an array");

    return pa;
}

DoTurnMessage parseDoTurnMessage(const json & object)
{
    DoTurnMessage m;

    const json & actions = object["player_actions"];
    m.playerActions.reserve(actions.size());

    for (unsigned int i = 0; i < actions.size(); i++)
    {
        m.playerActions.push_back(parsePlayerActions(actions[i]));
    }

    return m;
}

} // end of netorcai namespace

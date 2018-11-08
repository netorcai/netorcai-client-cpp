#pragma once

#include <string>
#include <vector>

#include "json.hpp"

namespace netorcai
{
    /// Stores information about one player
    struct PlayerInfo
    {
        int playerID; /// The player unique identifier (in [0..nbPlayers[)
        std::string nickname; /// The player nickname
        std::string remoteAddress; /// The player socket remote address
        bool isConnected; /// Whether the player is currently connected or not
    };

    PlayerInfo parsePlayerInfo(const json & object);

    /// Parses several player information (in GAME_STARTS and GAME_ENDS messages)
    std::vector<PlayerInfo> parsePlayersInfo(const json & array);

    /// Content of a LOGIN_ACK metaprotocol message
    struct LoginAckMessage
    {
        // ¯\_(ツ)_/¯
    };

    /// Content of a GAME_STARTS metaprotocol message
    struct GameStartsMessage
    {
        int playerID; /// Caller's player identifier. players: [0..nbPlayers[. visu: -1
        int nbPlayers; /// Number of players in the game
        int nbTurnsMax; /// Maximum number of turns. Game can finish before it
        double msBeforeFirstTurn; /// Time before the first TURN is sent (in ms)
        double msBetweenTurns; /// Time between two consecutive TURNs (in ms)
        std::vector<PlayerInfo> playersInfo; /// (only for visus) Information about the players
        nlohmann::json initialGameState; /// Game-dependent object.
    };

    GameStartsMessage parseGameStartsMessage(const json & json);
} // end of netorcai namespace

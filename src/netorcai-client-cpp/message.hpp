#pragma once

#include <string>
#include <vector>

#include <netorcai-client-cpp/json.hpp>

namespace netorcai
{
    /// Stores information about one player
    struct PlayerInfo
    {
        int playerID; //!< The player unique identifier (in [0..nbPlayers[)
        std::string nickname; //!< The player nickname
        std::string remoteAddress; //!< The player socket remote address
        bool isConnected; //!< Whether the player is currently connected or not
    };

    /// Content of a LOGIN_ACK metaprotocol message
    struct LoginAckMessage
    {
        // ¯\_(ツ)_/¯
    };

    /// Content of a GAME_STARTS metaprotocol message
    struct GameStartsMessage
    {
        int playerID; //!< Caller's player identifier. players: [0..nbPlayers+nbSpecialPlayers[. visu: -1
        int nbPlayers; //!< Number of players in the game
        int nbSpecialPlayers; //!< Number of special players in the game
        int nbTurnsMax; //!< Maximum number of turns. Game can finish before it
        double msBeforeFirstTurn; //!< Time before the first TURN is sent (in ms)
        double msBetweenTurns; //!< Time between two consecutive TURNs (in ms)
        std::vector<PlayerInfo> playersInfo; //!< (only for visus) Information about the players
        json initialGameState; //!< Game-dependent object.
    };

    /// Content of a GAME_ENDS metaprotocol message
    struct GameEndsMessage
    {
        int winnerPlayerID; //!< Unique identifier of the player that won the game. Or -1.
        json gameState; //!< Game-dependent object.
    };

    /// Content of a TURN metaprotocol message
    struct TurnMessage
    {
        int turnNumber; //!< In [0..nbTurnsMax[
        std::vector<PlayerInfo> playersInfo; //!< (only for visus) Information about the players
        json gameState; //!< Game-dependent object.
    };

    /// Content of a DO_INIT metaprotocol message
    struct DoInitMessage
    {
        int nbPlayers; //!< The number of players of the game
        int nbSpecialPlayers; //!< Number of special players in the game
        int nbTurnsMax; //!< The maximum number of turns of the game
    };

    /// Convenient struct for the player actions of a DO_TURN metaprotocol message
    struct PlayerActions
    {
        int playerID; //!< The identifier of the player that issued the actions
        int turnNumber; //!< The turn number the actions come from
        json actions; //!< The actions themselves
    };

    /// Content of a DO_TURN metaprotocol message
    struct DoTurnMessage
    {
        std::vector<PlayerActions> playerActions; //!< The ordered list of player actions
    };

    PlayerInfo parsePlayerInfo(const json & object);
    std::vector<PlayerInfo> parsePlayersInfo(const json & array);
    GameStartsMessage parseGameStartsMessage(const json & object);
    GameEndsMessage parseGameEndsMessage(const json & object);
    TurnMessage parseTurnMessage(const json & object);
    DoInitMessage parseDoInitMessage(const json & object);
    PlayerActions parsePlayerActions(const json & object);
    DoTurnMessage parseDoTurnMessage(const json & object);
} // end of netorcai namespace

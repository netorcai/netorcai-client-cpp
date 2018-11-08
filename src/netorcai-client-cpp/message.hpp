#pragma once

#include <string>

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
} // end of netorcai namespace

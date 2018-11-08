#include <gtest/gtest.h>

#include <string>

#include <netorcai-client-cpp/message.hpp>

using namespace std;
using namespace netorcai;

TEST(message, parsePlayerInfo)
{
    const string s = R"({
      "player_id": 0,
      "nickname": "jugador",
      "remote_address": "127.0.0.1:59840",
      "is_connected": true
    })";

    const PlayerInfo pinfo = parsePlayerInfo(json::parse(s));
    EXPECT_EQ(pinfo.playerID, 0);
    EXPECT_EQ(pinfo.nickname, "jugador");
    EXPECT_EQ(pinfo.remoteAddress, "127.0.0.1:59840");
    EXPECT_EQ(pinfo.isConnected, true);
}

TEST(message, parsePlayersInfo)
{
    const string s = R"([
      {
        "player_id": 0,
        "nickname": "jugador",
        "remote_address": "127.0.0.1:59840",
        "is_connected": true
      },
      {
        "player_id": 1,
        "nickname": "bot",
        "remote_address": "127.0.0.1:59842",
        "is_connected": false
      }
    ])";
    json array = json::parse(s);

    const std::vector<PlayerInfo> pinfos = parsePlayersInfo(json::parse(s));
    EXPECT_EQ(pinfos.size(), 2);

    EXPECT_EQ(pinfos[0].playerID, 0);
    EXPECT_EQ(pinfos[0].nickname, "jugador");
    EXPECT_EQ(pinfos[0].remoteAddress, "127.0.0.1:59840");
    EXPECT_EQ(pinfos[0].isConnected, true);

    EXPECT_EQ(pinfos[1].playerID, 1);
    EXPECT_EQ(pinfos[1].nickname, "bot");
    EXPECT_EQ(pinfos[1].remoteAddress, "127.0.0.1:59842");
    EXPECT_EQ(pinfos[1].isConnected, false);
}

TEST(message, parseGameStartsMessage)
{
    const string s = R"({
      "message_type": "GAME_STARTS",
      "player_id": 0,
      "players_info": [
        {
          "player_id": 0,
          "nickname": "jugador",
          "remote_address": "127.0.0.1:59840",
          "is_connected": true
        }
      ],
      "nb_players": 4,
      "nb_turns_max": 100,
      "milliseconds_before_first_turn": 1000,
      "milliseconds_between_turns": 1000,
      "initial_game_state": {}
    })";

    const GameStartsMessage m = parseGameStartsMessage(json::parse(s));
    EXPECT_EQ(m.playerID, 0);
    EXPECT_EQ(m.playersInfo.size(), 1);
    EXPECT_EQ(m.playersInfo[0].playerID, 0);
    EXPECT_EQ(m.playersInfo[0].nickname, "jugador");
    EXPECT_EQ(m.playersInfo[0].remoteAddress, "127.0.0.1:59840");
    EXPECT_EQ(m.playersInfo[0].isConnected, true);
    EXPECT_EQ(m.nbPlayers, 4);
    EXPECT_EQ(m.nbTurnsMax, 100);
    EXPECT_EQ(m.msBeforeFirstTurn, 1000);
    EXPECT_EQ(m.msBetweenTurns, 1000);
    EXPECT_EQ(m.initialGameState.size(), 0);
}

TEST(message, parseGamesEndsMessage)
{
    const string s = R"({
      "message_type": "GAME_ENDS",
      "winner_player_id": 0,
      "game_state": {}
    })";

    const GameEndsMessage m = parseGameEndsMessage(json::parse(s));
    EXPECT_EQ(m.winnerPlayerID, 0);
    EXPECT_EQ(m.gameState.size(), 0);
}

TEST(message, parseTurnMessage)
{
    const string s = R"({
      "message_type": "TURN",
      "turn_number": 0,
      "game_state": {},
      "players_info": [
        {
          "player_id": 0,
          "nickname": "jugador",
          "remote_address": "127.0.0.1:59840",
          "is_connected": true
        }
      ]
    })";

    const TurnMessage m = parseTurnMessage(json::parse(s));
    EXPECT_EQ(m.turnNumber, 0);
    EXPECT_EQ(m.gameState.size(), 0);
    EXPECT_EQ(m.playersInfo.size(), 1);
    EXPECT_EQ(m.playersInfo[0].playerID, 0);
    EXPECT_EQ(m.playersInfo[0].nickname, "jugador");
    EXPECT_EQ(m.playersInfo[0].remoteAddress, "127.0.0.1:59840");
    EXPECT_EQ(m.playersInfo[0].isConnected, true);
}

TEST(message, parseDoInitMessage)
{
    const string s = R"({
      "message_type": "DO_INIT",
      "nb_players": 4,
      "nb_turns_max": 100
    })";

    const DoInitMessage m = parseDoInitMessage(json::parse(s));
    EXPECT_EQ(m.nbPlayers, 4);
    EXPECT_EQ(m.nbTurnsMax, 100);
}

TEST(message, parsePlayerActions)
{
    const string s = R"({
      "player_id": 2,
      "turn_number": 4,
      "actions": []
    })";

    const PlayerActions pa = parsePlayerActions(json::parse(s));
    EXPECT_EQ(pa.playerID, 2);
    EXPECT_EQ(pa.turnNumber, 4);
    EXPECT_EQ(pa.actions.size(), 0);
}

TEST(message, parseDoTurnMessage)
{
    const string s = R"({
      "message_type": "DO_TURN",
      "player_actions": [
        {
          "player_id": 1,
          "turn_number": 2,
          "actions": []
        },
        {
          "player_id": 0,
          "turn_number": 3,
          "actions": [ 4 ]
        }
      ]
    })";

    DoTurnMessage m = parseDoTurnMessage(json::parse(s));
    EXPECT_EQ(m.playerActions.size(), 2);
    EXPECT_EQ(m.playerActions[0].playerID, 1);
    EXPECT_EQ(m.playerActions[0].turnNumber, 2);
    EXPECT_EQ(m.playerActions[0].actions.size(), 0);
    EXPECT_EQ(m.playerActions[1].playerID, 0);
    EXPECT_EQ(m.playerActions[1].turnNumber, 3);
    EXPECT_EQ(m.playerActions[1].actions.size(), 1);
    EXPECT_EQ(m.playerActions[1].actions[0], 4);
}

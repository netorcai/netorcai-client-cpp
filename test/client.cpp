#include <gtest/gtest.h>

#include <stdio.h>
#include <string>

#include <unistd.h>

#include <netorcai-client-cpp/client.hpp>

using namespace std;
using namespace netorcai;

FILE * launchNetorcaiWaitListening(int nbPlayers, int nbVisus)
{
    char * netorcaiCommand;
    FILE * netorcaiOutput;

    int ret = asprintf(&netorcaiCommand,
        "netorcai --simple-prompt "
                  "--delay-first-turn=50 "
                  "--delay-turns=50 "
                  "--nb-turns-max=2 "
                  "--nb-players-max=%d "
                  "--nb-visus-max=%d "
                  "--autostart",
        nbPlayers, nbVisus);
    EXPECT_NE(ret, -1) << "Cannot allocate netorcai command string";

    netorcaiOutput = popen(netorcaiCommand, "r");
    EXPECT_NE(netorcaiOutput, nullptr) << "Cannot execute subprocess";
    free(netorcaiCommand);

    // Wait for connection to be possible
    char * line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, netorcaiOutput);
    EXPECT_NE(read, -1) << "Cannot read netorcai's first output line";
    bool expected_first_line = (string::npos != ((string)line).find("Listening incoming connections"));
    EXPECT_TRUE(expected_first_line) <<
        "First netorcai's output is not about Listening. First line is printed below.\n" <<
        line;

    return netorcaiOutput;
}

TEST(client, everythingGoesWell)
{
    // Run netorcai
    FILE * n = launchNetorcaiWaitListening(1, 0);

    // Run game logic
    Client gameLogic;
    gameLogic.connect();
    gameLogic.sendLogin("gl", "game logic");
    gameLogic.readLoginAck();

    // Run player
    Client player;
    player.connect();
    player.sendLogin("player", "player");
    player.readLoginAck();

    // Game should start automatically as one player is connected (--autostart)
    const DoInitMessage doInit = gameLogic.readDoInit();
    gameLogic.sendDoInitAck(json::parse(R"({"all_clients": {"gl": "C++"}})"));
    player.readGameStarts();

    for (int i = 1; i < doInit.nbTurnsMax; i++)
    {
        gameLogic.readDoTurn();
        gameLogic.sendDoTurnAck(json::parse(R"({"all_clients": {"gl": "C++"}})"), -1);

        const TurnMessage turn = player.readTurn();
        player.sendTurnAck(turn.turnNumber, json::parse(R"([{"player": "C++"}])"));
    }

    gameLogic.readDoTurn();
    gameLogic.sendDoTurnAck(json::parse(R"({"all_clients": {"gl": "C++"}})"), -1);

    player.readGameEnds();
    int ret = pclose(n);
    EXPECT_NE(ret, -1) << "Error while calling pclose on netorcai's process";
}

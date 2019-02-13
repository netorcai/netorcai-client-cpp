#include <gtest/gtest.h>

#include <stdio.h>
#include <string>

#include <unistd.h>

#include <netorcai-client-cpp/client.hpp>
#include <netorcai-client-cpp/error.hpp>
#include <netorcai-client-cpp/version.hpp>

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
    EXPECT_TRUE(expected_first_line) << "First netorcai's output is not about Listening incoming connections...";

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

void getClientKicked(Client & client)
{
    client.connect();
    client.sendString("¿qué?");
}

TEST(client, kickedInsteadOfExpectedMessage)
{
    FILE * n = launchNetorcaiWaitListening(10, 20); // not autostarting here

    Client c;
    getClientKicked(c); EXPECT_THROW(c.readLoginAck(), netorcai::Error);
    getClientKicked(c); EXPECT_THROW(c.readGameStarts(), netorcai::Error);
    getClientKicked(c); EXPECT_THROW(c.readTurn(), netorcai::Error);
    getClientKicked(c); EXPECT_THROW(c.readGameEnds(), netorcai::Error);
    getClientKicked(c); EXPECT_THROW(c.readDoInit(), netorcai::Error);
    getClientKicked(c); EXPECT_THROW(c.readDoTurn(), netorcai::Error);

    system("killall netorcai");
    int ret = pclose(n);
    EXPECT_NE(ret, -1) << "Error while calling pclose on netorcai's process";
}

void getPlayerLogged(Client & client)
{
    client.connect();
    client.sendLogin("I", "player");
}

TEST(client, unexpectedMessageButNotKICK)
{
    auto n = launchNetorcaiWaitListening(2, 0);

    // LOGIN_ACK instead of something else
    Client c;
    getPlayerLogged(c); EXPECT_THROW(c.readGameStarts(), netorcai::Error);
    getPlayerLogged(c); EXPECT_THROW(c.readTurn(), netorcai::Error);
    getPlayerLogged(c); EXPECT_THROW(c.readGameEnds(), netorcai::Error);
    getPlayerLogged(c); EXPECT_THROW(c.readDoInit(), netorcai::Error);
    getPlayerLogged(c); EXPECT_THROW(c.readDoTurn(), netorcai::Error);
    c.close();

    // Start a game.
    Client player1, player2, gl;
    getPlayerLogged(player1); // Unexpected msg while reading LOGIN_ACK
    getPlayerLogged(player2); // GAME_ENDS while reading TURN
    gl.connect();
    gl.sendLogin("gl", "game logic");

    gl.readLoginAck();
    player1.readLoginAck();
    player2.readLoginAck();

    // Game should start automatically as two players are connected (--autostart)
    const DoInitMessage doInit = gl.readDoInit();
    gl.sendDoInitAck(json::parse(R"({"all_clients": {"gl": "C++"}})"));

    EXPECT_THROW(player1.readLoginAck(), netorcai::Error);
    player2.readGameStarts();

    for (int i = 1; i < doInit.nbTurnsMax; i++)
    {
        gl.readDoTurn();
        gl.sendDoTurnAck(json::parse(R"({"all_clients": {"gl": "C++"}})"), -1);

        auto turn = player2.readTurn();
        player2.sendTurnAck(turn.turnNumber, json::parse(R"([{"player": "C++"}])"));
    }

    gl.readDoTurn();
    gl.sendDoTurnAck(json::parse(R"({"all_clients": {"gl": "C++"}})"), -1);

    EXPECT_THROW(player2.readTurn(), netorcai::Error);

    system("killall netorcai");
    int ret = pclose(n);
    EXPECT_NE(ret, -1) << "Error while calling pclose on netorcai's process";
}

TEST(client, nonBlockingRecv)
{
    auto n = launchNetorcaiWaitListening(2, 0);

    Client gameLogic;
    gameLogic.connect();
    gameLogic.sendLogin("gl", "game logic");
    gameLogic.readLoginAck();

    Client player;
    player.connect();
    player.sendLogin("player", "player");

    // Reads LOGIN_ACK with the non-blocking API. Should be fine.
    string msg;
    bool received = player.recvStringNonBlocking(msg, 1000.0);
    EXPECT_TRUE(received);
    json msgJson = json::parse(msg);
    EXPECT_EQ(msgJson["message_type"], "LOGIN_ACK");

    // Waits for GAME_STARTS with the non-blocking API. Should timeout.
    received = player.recvStringNonBlocking(msg, 1000.0);
    EXPECT_FALSE(received);

    // Second player connects.
    Client player2;
    player2.connect();
    player2.sendLogin("player", "player");
    player2.readLoginAck();

    // Game should start automatically.
    gameLogic.readDoInit();
    gameLogic.sendDoInitAck(json::parse(R"({"all_clients": {"gl": "C++"}})"));

    // GAME_STARTS should be readable now.
    received = player.recvStringNonBlocking(msg, 1000.0);
    EXPECT_TRUE(received);
    msgJson = json::parse(msg);
    EXPECT_EQ(msgJson["message_type"], "GAME_STARTS");

    system("killall netorcai");
    int ret = pclose(n);
    EXPECT_NE(ret, -1) << "Error while calling pclose on netorcai's process";
}

TEST(client, nonCriticalNonMatchingMetaprotocolVersion)
{
    FILE * n = launchNetorcaiWaitListening(10, 20); // not autostarting here

    netorcai::minorVersion += 1;

    Client c;
    c.connect();
    c.sendLogin("cpptest", "player");
    c.readLoginAck();

    netorcai::minorVersion -= 1;

    system("killall netorcai");
    int ret = pclose(n);
    EXPECT_NE(ret, -1) << "Error while calling pclose on netorcai's process";
}

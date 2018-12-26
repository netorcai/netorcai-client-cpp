#include <stdio.h>

#include <netorcai-client-cpp/client.hpp>
#include <netorcai-client-cpp/error.hpp>

int main()
{
    using namespace netorcai;

    try
    {
        Client c;

        printf("Connecting to netorcai... "); fflush(stdout);
        c.connect();
        printf("done\n");

        printf("Logging in as a game logic... "); fflush(stdout);
        c.sendLogin("C++-gl", "game logic");
        c.readLoginAck();
        printf("done\n");

        printf("Waiting for DO_INIT... "); fflush(stdout);
        const DoInitMessage doInit = c.readDoInit();
        c.sendDoInitAck(json::parse(R"({"all_clients": {"gl": "C++"}})"));
        printf("done\n");

        for (int turn = 0; turn < doInit.nbTurnsMax; turn++)
        {
            printf("Waiting for DO_TURN %d... ", turn); fflush(stdout);
            const DoTurnMessage doTurn = c.readDoTurn();
            c.sendDoTurnAck(json::parse(R"({"all_clients": {"gl": "C++"}})"), -1);
            printf("done\n");
        }

        return 0;
    }
    catch (const netorcai::Error & e)
    {
        printf("Failure: %s\n", e.what());
        return 1;
    }
}

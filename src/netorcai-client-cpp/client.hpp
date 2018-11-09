#pragma once

#include <string>

#include <SFML/Network/TcpSocket.hpp>

#include <netorcai-client-cpp/json.hpp>
#include <netorcai-client-cpp/message.hpp>

namespace netorcai
{

class Client
{
private:
    sf::TcpSocket _socket;

public:
    explicit Client() = default;
    ~Client();

    void connect(const std::string & hostname = "localhost", unsigned short port = 4242);
    void close();

    std::string recvString();
    json recvJson();

    LoginAckMessage readLoginAck();
    GameStartsMessage readGameStarts();
    TurnMessage readTurn();
    GameEndsMessage readGameEnds();
    DoInitMessage readDoInit();
    DoTurnMessage readDoTurn();

    void sendString(const std::string & message);
    void sendJson(const json & message);

    void sendLogin(const std::string & nickname, const std::string & role);
    void sendTurnAck(int turnNumber, const json & actions);
    void sendDoInitAck(const json & initialGameState);
    void sendDoTurnAck(const json & gameState, int winnerPlayerID);
};

} // end of netorcai namespace

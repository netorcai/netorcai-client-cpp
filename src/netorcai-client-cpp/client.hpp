#pragma once

#include <string>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>

#include <netorcai-client-cpp/json.hpp>
#include <netorcai-client-cpp/message.hpp>

namespace netorcai
{

/// Netorcai metaprotocol client class (C++ version)
class Client
{
private:
    sf::TcpSocket _socket;
    sf::SocketSelector _socketSelector;

public:
    explicit Client() = default;
    ~Client();

    void connect(const std::string & hostname = "localhost", unsigned short port = 4242);
    void close();

    std::string recvString();
    bool recvStringNonBlocking(std::string & message, double millisecondsTimeout = 50);
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

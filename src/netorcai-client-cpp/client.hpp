#pragma once

#include <string>

#include <SFML/Network/TcpSocket.hpp>

#include <netorcai-client-cpp/json.hpp>

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

    void sendString(const std::string & message);
    void sendJson(const json & message);
};

} // end of netorcai namespace

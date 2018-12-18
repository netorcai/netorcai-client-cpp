#include <netorcai-client-cpp/client.hpp>
#include <netorcai-client-cpp/error.hpp>

#include <netorcai-client-cpp/endian.hpp>
#include <utility>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Socket.hpp>

namespace netorcai
{

Client::~Client()
{
    close();
}

/// Connect to a remote endpoint. May throw Error.
/// @param hostname Endpoint's host name (e.g., localhost, 127.0.0.1...)
/// @param port Endpoint's TCP port.
void Client::connect(const std::string & hostname, unsigned short port)
{
    auto status = _socket.connect(sf::IpAddress(hostname), port, sf::milliseconds(500));
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot connect to %s:%hu", hostname.c_str(), port);
    _socketSelector.add(_socket);
}

/// Close the socket.
void Client::close()
{
    _socket.disconnect();
}

/// Reads a string message on the client socket. May throw Error.
/// @return The message that has been read.
std::string Client::recvString()
{
    // Read content size
    uint8_t contentSizeBuf[2];
    std::size_t receivedSize;
    auto status = _socket.receive((void*)contentSizeBuf, 2, receivedSize);
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot read content size.");
    NETORCAI_ENFORCE(receivedSize == 2, "Cannot read content size.");

    // Convert from little endian to native endian if needed
    if constexpr (netorcai::endian::native == netorcai::endian::big)
    {
        std::swap(contentSizeBuf[0], contentSizeBuf[1]);
    }

    const uint16_t contentSize = *((uint16_t*)contentSizeBuf);

    // Read content
    std::string content;
    content.resize(contentSize);
    status = _socket.receive((void*)content.data(), contentSize, receivedSize);
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot read content.");
    NETORCAI_ENFORCE(receivedSize == contentSize, "Cannot read content.");

    return content;
}

/// Reads a string message on the client socket with a given timeout. May throw Error.
/// @param[out] message The message read.
/// @param[in] millisecondsTimeout The maximum time allowed before receiving a message.
/// @details This function may take more than millisecondsTimeout to return.
///          If a message size could be read before timeout, the full message reception will be waited for.
/// @return If a message could be read, true is returned and message is set.
///         Otherwise, false is returned and message="".
bool Client::recvStringNonBlocking(std::string & message, double millisecondsTimeout)
{
    if (_socketSelector.wait(sf::milliseconds(millisecondsTimeout)))
    {
        message = recvString();
        return true;
    }

    message = "";
    return false;
}

/// Reads a JSON message on the client socket. May throw Error.
/// @return The message that has been read.
json Client::recvJson()
{
    return json::parse(recvString());
}

/// Reads a LOGIN_ACK message on the client socket. May throw Error.
/// @return The LOGIN_ACK message that has been read.
LoginAckMessage Client::readLoginAck()
{
    json msg = recvJson();
    if (msg["message_type"] == "LOGIN_ACK")
        return LoginAckMessage();
    else if (msg["message_type"] == "KICK")
        throw Error("Kicked from netorai. Reason: %s", ((std::string)msg["kick_reason"]).c_str());
    else
        throw Error("Unexpected message received: %s", ((std::string)msg["message_type"]).c_str());
}

/// Reads a GAME_STARTS message on the client socket. May throw Error.
/// @return The GAME_STARTS message that has been read.
GameStartsMessage Client::readGameStarts()
{
    json msg = recvJson();
    if (msg["message_type"] == "GAME_STARTS")
        return parseGameStartsMessage(msg);
    else if (msg["message_type"] == "KICK")
        throw Error("Kicked from netorai. Reason: %s", ((std::string)msg["kick_reason"]).c_str());
    else
        throw Error("Unexpected message received: %s", ((std::string)msg["message_type"]).c_str());
}

/// Reads a TURN message on the client socket. May throw Error.
/// @return The TURN message that has been read.
TurnMessage Client::readTurn()
{
    json msg = recvJson();
    if (msg["message_type"] == "TURN")
        return parseTurnMessage(msg);
    if (msg["message_type"] == "GAME_ENDS")
        throw Error("Game over!");
    else if (msg["message_type"] == "KICK")
        throw Error("Kicked from netorai. Reason: %s", ((std::string)msg["kick_reason"]).c_str());
    else
        throw Error("Unexpected message received: %s", ((std::string)msg["message_type"]).c_str());
}

/// Reads a GAME_ENDS message on the client socket. May throw Error.
/// @return The GAME_ENDS message that has been read.
GameEndsMessage Client::readGameEnds()
{
    json msg = recvJson();
    if (msg["message_type"] == "GAME_ENDS")
        return parseGameEndsMessage(msg);
    else if (msg["message_type"] == "KICK")
        throw Error("Kicked from netorai. Reason: %s", ((std::string)msg["kick_reason"]).c_str());
    else
        throw Error("Unexpected message received: %s", ((std::string)msg["message_type"]).c_str());
}

/// Reads a DO_INIT message on the client socket. May throw Error.
/// @return The DO_INIT message that has been read.
DoInitMessage Client::readDoInit()
{
    json msg = recvJson();
    if (msg["message_type"] == "DO_INIT")
        return parseDoInitMessage(msg);
    else if (msg["message_type"] == "KICK")
        throw Error("Kicked from netorai. Reason: %s", ((std::string)msg["kick_reason"]).c_str());
    else
        throw Error("Unexpected message received: %s", ((std::string)msg["message_type"]).c_str());
}

/// Reads a DO_TURN message on the client socket. May throw Error.
/// @return The DO_TURN message that has been read.
DoTurnMessage Client::readDoTurn()
{
    json msg = recvJson();
    if (msg["message_type"] == "DO_TURN")
        return parseDoTurnMessage(msg);
    else if (msg["message_type"] == "KICK")
        throw Error("Kicked from netorai. Reason: %s", ((std::string)msg["kick_reason"]).c_str());
    else
        throw Error("Unexpected message received: %s", ((std::string)msg["message_type"]).c_str());
}

/// Send a string message on the client socket. May throw Error.
/// @param message The content to write on the socket.
void Client::sendString(const std::string & message)
{
    NETORCAI_ENFORCE(message.size() < 65536, "message is too big (%zu bytes)", message.size());

    uint16_t contentSize = message.size();
    uint8_t contentSizeBuf[2];
    *((uint16_t*)contentSizeBuf) = contentSize;

    // Convert from native endian to little endian if needed
    if constexpr (netorcai::endian::native == netorcai::endian::big)
    {
        std::swap(contentSizeBuf[0], contentSizeBuf[1]);
    }

    size_t sentSize;
    auto status = _socket.send((void*)contentSizeBuf, 2, sentSize);
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot send content size.");
    NETORCAI_ENFORCE(sentSize == 2, "Cannot send content size.");

    status = _socket.send((void*)message.data(), contentSize, sentSize);
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot send content.");
    NETORCAI_ENFORCE(sentSize == contentSize, "Cannot send content.");
}

/// Send a JSON message on the client socket. May throw Error.
/// @param message The content to write on the socket.
void Client::sendJson(const json & message)
{
    sendString(message.dump());
}

/// Send a LOGIN message on the client socket. May throw Error.
/// @param nickname The desired nickname.
/// @param role The desired role ("player", "visualization" or "game logic").
void Client::sendLogin(const std::string & nickname, const std::string & role)
{
    json msg;
    msg["message_type"] = "LOGIN";
    msg["nickname"] = nickname;
    msg["role"] = role;

    sendJson(msg);
}

/// Send a TURN_ACK message on the client socket. May throw Error.
/// @param turnNumber The number of the TURN message this ACK refers to.
/// @param actions The decided actions.
void Client::sendTurnAck(int turnNumber, const json & actions)
{
    json msg;
    msg["message_type"] = "TURN_ACK";
    msg["turn_number"] = turnNumber;
    msg["actions"] = actions;

    sendJson(msg);
}

/// Send a DO_INIT_ACK message on the client socket. May throw Error.
/// @param initialGameState The initial game state.
void Client::sendDoInitAck(const json & initialGameState)
{
    json msg;
    msg["message_type"] = "DO_INIT_ACK";
    msg["initial_game_state"] = initialGameState;

    sendJson(msg);
}

/// Send a DO_TURN_ACK message on the client socket. May throw Error.
/// @param gameState The current game state.
/// @param winnerPlayerID The id of the current winner (or -1).
void Client::sendDoTurnAck(const json & gameState, int winnerPlayerID)
{
    json msg;
    msg["message_type"] = "DO_TURN_ACK";
    msg["winner_player_id"] = winnerPlayerID;
    msg["game_state"] = gameState;

    sendJson(msg);
}

} // end of netorcai namespace

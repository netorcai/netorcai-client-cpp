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

}

void Client::connect(const std::string & hostname, unsigned short port)
{
    auto status = _socket.connect(sf::IpAddress(hostname), port, sf::milliseconds(500));
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot connect to %s:%hu", hostname.c_str(), port);
}

void Client::close()
{
    _socket.disconnect();
}

std::string Client::recvString()
{
    // Read content size
    uint8_t contentSizeBuf[2];
    std::size_t receivedSize;
    auto status = _socket.receive((void*)contentSizeBuf, 2, receivedSize);
    NETORCAI_ENFORCE(status == sf::Socket::Done, "Cannot read content size.");
    NETORCAI_ENFORCE(receivedSize == 2, "Cannot read content size.");

    // Convert from little endian to native endian if needed
    if constexpr (std::endian::native == std::endian::big)
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

json Client::recvJson()
{
    return json::parse(recvString());
}

void Client::sendString(const std::string & message)
{
    NETORCAI_ENFORCE(message.size() < 65536, "message is too big (%zu bytes)", message.size());

    uint16_t contentSize = message.size();
    uint8_t contentSizeBuf[2];
    *((uint16_t*)contentSizeBuf) = contentSize;

    // Convert from native endian to little endian if needed
    if constexpr (std::endian::native == std::endian::big)
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

void Client::sendJson(const json & message)
{
    sendString(message.dump());
}

} // end of netorcai namespace

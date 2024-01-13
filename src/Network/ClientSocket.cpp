#include "ClientSocket.hpp"
#include <iostream>

ClientSocket::ClientSocket(const ClientSocket::SocketSettings& settings)
    : m_Socket(settings.clientSocket), m_Port(settings.port), m_RemoteIP(settings.ip)
{
}

ClientSocket::~ClientSocket()
{
    closesocket(m_Socket);
}

void ClientSocket::Send(const std::string& data)
{
    if(send(m_Socket, data.c_str(), data.size(), 0) == SOCKET_ERROR)
    {
	std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
    }
}

std::string ClientSocket::Receive()
{
    constexpr u16 DEFAULT_BUFLEN = 2048;

    char buffer[DEFAULT_BUFLEN];
    int result = recv(m_Socket, buffer, sizeof(buffer), 0);
    if(result == SOCKET_ERROR)
    {
	std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
    }

    return {buffer, static_cast<size_t>(result)};
}

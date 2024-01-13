#pragma once

#include "../Defines.hpp"

// clang-format off
#include <winsock2.h>
// clang-format on

#include <string>

class ClientSocket final
{
    // client socket holds the socket of the client that is connected to the server
    // and the address of the client
    // and references to the socket of the server and the address of the server

  public:
    struct SocketSettings
    {
	u16 port;
	std::string ip;
	SOCKET clientSocket;
	sockaddr_in serverAddr;

	SocketSettings(u16 port, const std::string& ip, SOCKET clientSocket, sockaddr_in serverAddr)
	    : port(port), ip(ip), clientSocket(clientSocket), serverAddr(serverAddr)
	{
	}
    };

    ClientSocket(const SocketSettings& settings);
    ~ClientSocket();

    void Send(const std::string& data);
    std::string Receive();

  private:
    SOCKET m_Socket;

    u16 m_Port;
    std::string m_RemoteIP;
    sockaddr_in m_ServerAddr;
};

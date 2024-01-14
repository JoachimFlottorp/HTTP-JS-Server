#pragma once

#include "../Defines.hpp"

#include <winsock2.h>

#include <string>
#include <utility>

class ClientSocket final
{
  public:
    struct SocketSettings
    {
	u16 port;
	std::string ip;
	SOCKET clientSocket;
	sockaddr_in serverAddr;

	SocketSettings(u16 port, std::string ip, SOCKET clientSocket, sockaddr_in serverAddr)
	    : port(port), ip(std::move(ip)), clientSocket(clientSocket), serverAddr(serverAddr)
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

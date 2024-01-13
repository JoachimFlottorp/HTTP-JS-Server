#pragma once

#define WIN32_LEAN_AND_MEAN

#include "../Defines.hpp"
#include "ClientSocket.hpp"

// clang-format off
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// clang-format on

class ListenSocket final
{
  public:
    struct SocketSettings
    {
	u16 port;

	explicit SocketSettings(u16 port) : port(port) {}
    };

    explicit ListenSocket(const SocketSettings& settings);
    ~ListenSocket();

    void Listen();
    ClientSocket Accept();

  private:
    SOCKET m_Socket = INVALID_SOCKET;
    sockaddr_in m_addr = {};
};

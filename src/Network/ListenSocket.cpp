#include "ListenSocket.hpp"

#include <iostream>

struct SocketInitializer
{
				SocketInitializer()
				{
								WSADATA wsaData;

								i32 result = WSAStartup(MAKEWORD(2, 2), &wsaData);
								if(result != 0)
								{
												std::cout << "WSAStartup failed with error: " << result << std::endl;
												std::exit(1);
								}
				}

				~SocketInitializer() { WSACleanup(); }
};

SocketInitializer g_socketInitializer;

ListenSocket::ListenSocket(const SocketSettings& settings)
{
				m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if(m_Socket == INVALID_SOCKET)
				{
								std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
								std::exit(1);
				}

				m_addr.sin_family = AF_INET;
				m_addr.sin_port = htons(settings.port);
				m_addr.sin_addr.s_addr = INADDR_ANY; // For future. Change this to set loopback only.
}

ListenSocket::~ListenSocket()
{
				closesocket(m_Socket);
}

void ListenSocket::Listen()
{
				if(bind(m_Socket, (sockaddr*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
				{
								std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
								std::exit(1);
				}

				if(listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
				{
								std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
								std::exit(1);
				}
}

std::optional<ClientSocket> ListenSocket::Accept()
{
				SOCKET clientSocket = accept(m_Socket, nullptr, nullptr);
				if(clientSocket == INVALID_SOCKET)
				{
								std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
								std::exit(1);

								// FIXME: Need some better error handling.
				}

				std::string ip;
				auto cIp = InetNtop(m_addr.sin_family, &m_addr.sin_addr, nullptr, INET_ADDRSTRLEN);
				if(!cIp)
				{
								cIp = "<unknown>";

								std::cout << "InetNtop failed with error: " << WSAGetLastError() << std::endl;
				}
				else
								ip = cIp;

				ClientSocket::SocketSettings settings(m_addr.sin_port, ip, clientSocket, m_addr);
				return {settings};
}

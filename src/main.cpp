#include "Defines.hpp"
#include "Network/ListenSocket.hpp"

#include <iostream>

int main()
{
    constexpr auto port = 27015;

    ListenSocket socket {ListenSocket::SocketSettings(port)};
    socket.Listen();

    std::cout << "Listening on port " << port << std::endl;

    auto client = socket.Accept();
    std::cout << "Client connected" << std::endl;

    auto message = client.Receive();
    std::cout << "Received: " << message << std::endl;

    client.Send("Hello from server!");

    return 0;
}

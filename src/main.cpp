#include "Defines.hpp"
#include "HTTP/HTTPRequestParser.hpp"
#include "Network/ListenSocket.hpp"

#include <iostream>

int main()
{
    constexpr auto port = 27015;

    ListenSocket socket {ListenSocket::SocketSettings(port)};
    socket.Listen();

    std::cout << "Listening on port " << port << std::endl;

    bool running = true;
    while(running)
    {
	// FIXME: oClient and client will call destructors twice.
	auto oClient = socket.Accept();
	if(!oClient.has_value())
	    continue;

	auto client = oClient.value();

	std::cout << "Client connected" << std::endl;

	auto message = client.Receive();
	std::cout << "Received: " << message << std::endl;

	HTTPRequest request;
	auto parseResult = HTTPRequestParser::Parse(request, message);
	if(parseResult != HTTPRequestParser::ParseResult::Success)
	{
	    std::cout << "Failed to parse request " << static_cast<u32>(parseResult) << std::endl;
	    continue;
	    // FIXME: Send 400 Bad Request
	}

	request.DebugLog();

	auto xHeader = request.GetHeader("x-test");
	if(!xHeader.empty())
	{
	    std::cout << "x-test: " << xHeader << std::endl;
	}

	client.Send("HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    }

    return 0;
}

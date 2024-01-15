#include "Defines.hpp"
#include "HTTP/HTTPRequestParser.hpp"
#include "HTTP/HTTPResponse.hpp"
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

	auto xHeader = request.GetHeaders().Get("x-test");
	if(!xHeader.empty())
	{
	  std::cout << "x-test: " << xHeader << std::endl;
	}

	auto response = HTTPResponse()
						.SetStatusCode(200)						 //
						.SetHeader("Content-Type", "text/plain") //
						.SetBody("Hello, World!");

	client.Send(response.ToString());
  }

  return 0;
}

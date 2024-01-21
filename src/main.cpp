#include "HTTP/HTTPRequestParser.hpp"
#include "HTTP/HTTPResponse.hpp"
#include "HTTP/HTTPStatus.hpp"
#include "Network/ListenSocket.hpp"
#include "Scripting/ScriptEngine.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

struct Route
{
  typedef std::function<void(const HTTPRequest&, HTTPResponse&)> HandlerFunc;

  std::string path;
  HTTPMethod method;
  HandlerFunc handler;
};

class Router
{
public:
  Router() = default;
  ~Router() = default;

  void AddRoute(const std::string& path, HTTPMethod method, Route::HandlerFunc handler)
  {
	m_Routes.push_back({path, method, std::move(handler)});
  }

  void HandleRequest(const HTTPRequest& request, HTTPResponse& response)
  {
	// FIXME: This should support trie matching.
	// 		  https://ayende.com/blog/173313/trie-based-routing

	for(auto& route : m_Routes)
	{
	  if(route.method != request.GetMethod())
		continue;

	  if(route.path != request.GetURI())
		continue;

	  route.handler(request, response);
	  return;
	}

	response.SetStatusCode(HTTPStatus::NotFound);
  }

private:
  std::vector<Route> m_Routes;
};

void index(const HTTPRequest& request, HTTPResponse& response)
{
  response
	  .SetStatusCode(200)					   //
	  .SetHeader("Content-Type", "text/plain") //
	  .SetBody("Hello, World!");
}

void test(const HTTPRequest& request, HTTPResponse& response)
{
  response
	  .SetStatusCode(200)					   //
	  .SetHeader("Content-Type", "text/plain") //
	  .SetBody("Hello, Test!");
}

void eval(const HTTPRequest& request, HTTPResponse& response)
{
  auto script = request.GetBody();

  ScriptEngine::The()->Execute(script);

  response
	  .SetStatusCode(200)					   //
	  .SetHeader("Content-Type", "text/plain") //
	  .SetBody("Hello, Eval!");
}

int main()
{
  constexpr auto port = 27015;

  ScriptEngine::Initialize();

  ListenSocket socket {ListenSocket::SocketSettings(port)};
  socket.Listen();

  Router router;
  router.AddRoute("/", HTTPMethod::GET, index);
  router.AddRoute("/test", HTTPMethod::GET, test);
  router.AddRoute("/eval", HTTPMethod::POST, eval);

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

	HTTPResponse response;
	router.HandleRequest(request, response);

	client.Send(response.ToString());
  }

  return 0;
}

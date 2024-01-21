#include "HTTP/HTTPRequestParser.hpp"
#include "HTTP/HTTPResponse.hpp"
#include "HTTP/HTTPStatus.hpp"
#include "Network/ListenSocket.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "dukglue/dukglue.h"

#include <iostream>

void eval(const HTTPRequest& request, HTTPResponse& response)
{
  const auto& script = request.GetBody();

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
  auto scriptEngine = ScriptEngine::The();
  auto ctx = scriptEngine->GetContext();

  dukglue_register_method(ctx.get(), &HTTPResponse::SetText, "setText");

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

	HTTPResponse response;
	if(request.GetURI() == "/eval")
	{
	  eval(request, response);
	}
	else
	{
	  const auto& routers = ScriptEngine::The()->GetRouters();

	  // Find the router that begins with the incoming request's URI. But not the '/' router
	  // because that's the default router.
	  auto router = std::find_if(routers.begin(), routers.end(), [&request](const auto& router)
								 { return request.GetURI().find(router.first) == 0 && router.first != "/"; });

	  if(router != routers.end())
	  {
		std::cout << "Found router " << router->first << std::endl;

		auto pathRoutes = router->second->GetRoutes();
		// If the router is found, then we need to strip the prefix from the URI. and find the sub-function that matches
		// the tail.
		auto tail = request.GetURI().substr(router->first.size());
		std::cout << "Tail: " << tail << std::endl;
		if(tail.empty())
		  tail = "/";

		// FIXME: This should support trie matching.
		// 		  https://ayende.com/blog/173313/trie-based-routing

		auto route = std::find_if(pathRoutes.begin(), pathRoutes.end(),
								  [&tail](const auto& route) { return route.path == tail; });

		if(route != pathRoutes.end())
		{
		  std::cout << "Found route " << route->path << std::endl;

		  // Now to call the JavaScript handler.
		  auto ctx = ScriptEngine::The()->GetContext();
		  duk_push_global_object(ctx.get());
		  duk_get_prop_string(ctx.get(), -1, "__functionStore");
		  duk_get_prop_index(ctx.get(), -1, route->handlerIdx);

		  // FIXME: Is placing a stack-allocated object into JavaScript land a bad idea?
		  dukglue_push(ctx.get(), &request);
		  dukglue_push(ctx.get(), &response);

		  i32 result = duk_pcall(ctx.get(), 2);
		  if(result != DUK_EXEC_SUCCESS)
		  {
			std::cout << "Failed to execute JavaScript: " << duk_safe_to_string(ctx.get(), -1) << std::endl;
			duk_pop(ctx.get());
		  }

		  duk_pop(ctx.get());
		  duk_pop(ctx.get());
		  duk_pop(ctx.get());
		}
		else
		{
		  std::cout << "No route found for " << request.GetURI() << std::endl;

		  response.SetStatusCode(404);
		}
	  }
	  else
	  {
		std::cout << "No router found for " << request.GetURI() << std::endl;

		response.SetStatusCode(404);
	  }
	}

	// FIXME: If the body is empty. then automatically add generic message matching status code.
	client.Send(response.ToString());
  }

  return 0;
}

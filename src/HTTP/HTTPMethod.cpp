#include "HTTPMethod.hpp"
#include <map>

const std::string& HTTPMethodToString(HTTPMethod method)
{
  static std::string unknown = "UNKNOWN";
  static std::map<HTTPMethod, std::string> methods = {
      // clang-format off
	{HTTPMethod::OPTIONS, "OPTIONS"},
	{HTTPMethod::GET, "GET"},
	{HTTPMethod::HEAD, "HEAD"},
	{HTTPMethod::POST, "POST"},
	{HTTPMethod::PUT, "PUT"},
	{HTTPMethod::DELETE, "DELETE"},
	{HTTPMethod::TRACE, "TRACE"},
	{HTTPMethod::CONNECT, "CONNECT"},
      // clang-format on
  };

  if(methods.find(method) != methods.end())
    return methods[method];
  else
    return unknown;
}

HTTPMethod HTTPMethodFromString(const std::string& string)
{
  static std::map<std::string, HTTPMethod> methods = {
      // clang-format off
	{"OPTIONS", HTTPMethod::OPTIONS},
	{"GET", HTTPMethod::GET},
	{"HEAD", HTTPMethod::HEAD},
	{"POST", HTTPMethod::POST},
	{"PUT", HTTPMethod::PUT},
	{"DELETE", HTTPMethod::DELETE},
	{"TRACE", HTTPMethod::TRACE},
	{"CONNECT", HTTPMethod::CONNECT},
      // clang-format on
  };

  return methods[string];
}
#pragma once

#include "../Defines.hpp"
#include "HTTPRequest.hpp"

#include <string>

class HTTPRequestParser
{
public:
  enum class ParseResult
  {
	Success,
	InvalidRequestLine,
	InvalidMethod,
	InvalidURI,
	InvalidVersion,
	InvalidBody
  };

  static ParseResult Parse(HTTPRequest& req, const std::string& data);

private:
  HTTPRequestParser(HTTPRequest& req, std::string data);

  ParseResult ParseRequestLine();
  ParseResult ParseHeaders();
  ParseResult ParseBody();

  bool CheckBody();

  std::string ConsumeUntil(char c);
  std::string ConsumeUntil(const std::string& str);

  bool ExpectCRLF();

private:
  HTTPRequest& m_Request;
  std::string m_Data;
  u32 m_Index {0};
  u32 m_BodySize {0};
}; // namespace HTTPRequestParser

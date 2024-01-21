#pragma once

#include "HTTPHeaders.hpp"

struct HTTPStatusLine
{
  std::string version = "HTTP/1.1";
  int statusCode = 200;
  std::string reasonPhrase {};

  HTTPStatusLine() = default;
  HTTPStatusLine(std::string version, int statusCode, std::string reasonPhrase);
};

class HTTPResponse
{
public:
  HTTPResponse() = default;
  HTTPResponse(HTTPStatusLine statusLine, HTTPHeaders headers, std::string body);

  ~HTTPResponse() = default;

  [[nodiscard]] std::string ToString() const;

  HTTPResponse& SetStatusCode(int statusCode);

  HTTPHeaders& GetHeaders();
  HTTPResponse& SetHeader(const std::string& key, const std::string& value);

  HTTPResponse& SetText(const std::string& text);
  HTTPResponse& SetBody(std::string body);
  HTTPResponse& AddBody(const std::string& body);

private:
  HTTPStatusLine m_StatusLine;
  HTTPHeaders m_Headers;
  std::string m_Body;
};

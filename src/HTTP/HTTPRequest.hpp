#pragma once

#include "HTTPHeaders.hpp"
#include "HTTPMethod.hpp"

#include <map>
#include <string>

struct HTTPRequestLine
{
  // FIXME: Better defaults.
  HTTPMethod method = HTTPMethod::GET;
  std::string uri = "/";
  std::string version = "HTTP/1.1";

  HTTPRequestLine();
  HTTPRequestLine(HTTPMethod method, std::string uri, std::string version);
};

class HTTPRequest
{
public:
  HTTPRequest();
  HTTPRequest(HTTPRequestLine requestLine, HTTPHeaders headers, std::string body);

  ~HTTPRequest();

  [[nodiscard]] HTTPMethod GetMethod() const { return m_RequestLine.method; }
  [[nodiscard]] std::string GetURI() const { return m_RequestLine.uri; }
  HTTPHeaders& GetHeaders() { return m_Headers; }
  const std::string& GetBody() const { return m_Body; }

  [[nodiscard]] std::string ToString() const;

  void DebugLog() const;

private:
  HTTPRequestLine m_RequestLine;
  HTTPHeaders m_Headers;
  std::string m_Body;

  friend class HTTPRequestParser;
};

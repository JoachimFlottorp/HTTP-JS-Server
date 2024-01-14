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

    HTTPMethod GetMethod() const { return m_RequestLine.method; }
    HTTPHeaders& GetHeaders() { return m_Headers; }

    [[nodiscard]] std::string ToString() const;

    void DebugLog() const;

  private:
    HTTPRequestLine m_RequestLine;
    HTTPHeaders m_Headers;
    std::string m_Body;

    friend class HTTPRequestParser;
};
